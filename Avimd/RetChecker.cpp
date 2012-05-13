/*
 * S2E Selective Symbolic Execution Framework
 *
 * Copyright (c) 2010, Dependable Systems Laboratory, EPFL
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the Dependable Systems Laboratory, EPFL nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE DEPENDABLE SYSTEMS LABORATORY, EPFL BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * Currently maintained by:
 *    Vitaly Chipounov <vitaly.chipounov@epfl.ch>
 *    Volodymyr Kuznetsov <vova.kuznetsov@epfl.ch>
 *
 * All contributors are listed in S2E-AUTHORS file.
 *
 */

extern "C" {
#include "config.h"
#include "qemu-common.h"
}

#include "RetChecker.h"
#include <s2e/S2E.h>
#include <s2e/ConfigFile.h>
#include <s2e/Utils.h>

#include <iostream>
#include <stack>

namespace s2e {
namespace plugins {

S2E_DEFINE_PLUGIN(RetChecker, "RetChecker S2E plugin", "", "ModuleExecutionDetector");

class RetCheckerState : public PluginState
{
public:
    typedef klee::ref<klee::Expr> Ret;
    typedef std::stack<Ret> RetStack;
    enum RetCheckerResult {
        RCR_OK=0, RCR_UNDERFLOW, RCR_MISMATCH
    };
    void storeReturnAddress(Ret actualRet);
    RetCheckerResult checkReturnAddress(Ret actualRet, S2EExecutionState *state = NULL, bool verbose = true);

private:
    RetStack m_stack;
    RetChecker *m_plugin;
    bool retvalEqual(Ret a, Ret b);
public:
    RetCheckerState();
    virtual ~RetCheckerState();
    virtual RetCheckerState* clone() const;
    static PluginState *factory(Plugin *p, S2EExecutionState *s);
};


RetCheckerState::RetCheckerState()
{

}

RetCheckerState::~RetCheckerState()
{

}

PluginState *RetCheckerState::factory(Plugin *p, S2EExecutionState *s)
{
    RetCheckerState *ret = new RetCheckerState();
    ret->m_plugin = static_cast<RetChecker*>(p);
    return ret;
}

RetCheckerState* RetCheckerState::clone() const
{
    return new RetCheckerState(*this);
}

RetCheckerState::RetCheckerResult RetCheckerState::checkReturnAddress(RetCheckerState::Ret actualRet, S2EExecutionState *state, bool verbose)
{
    if (m_stack.empty()) {
        if (verbose)
            m_plugin->s2e()->getMessagesStream(state) << "RetCheck failed: stack underflow\n";
        return RCR_UNDERFLOW;
    } else {
        Ret savedRet = m_stack.top();
        m_stack.pop();
        if (savedRet == actualRet) {
            if (verbose) {
                m_plugin->s2e()->getMessagesStream(state) << "RetCheck succeded\n";
                if (m_stack.empty() && verbose) {
                    m_plugin->s2e()->getMessagesStream(state) << "Leaving block started from entryPoint\n";
                }
            }
            return RCR_OK;
        } else {
            m_plugin->s2e()->getMessagesStream(state) << "RetCheck failed: mismatch: "
                    << savedRet << " expected " << " and " << actualRet << " Given\n";
            return RCR_MISMATCH;
        }
    }
}

void RetCheckerState::storeReturnAddress(RetCheckerState::Ret intendedRet)
{
    m_stack.push(intendedRet);
}

bool RetCheckerState::retvalEqual(Ret a, Ret b)
{
    //XXX hack: we use the fact Ret is ref<Expr>
    //YYY may be not need 'cause we can read only 32bits we need for saved retAddr
    //(==) check is not enough because Rets can be constants of different length
    if (a == b)
        return true; //Never seen to happen
    /*
    klee::Expr::Kind ka = a.get()->getKind();
    klee::Expr::Kind kb = b.get()->getKind();
    //if (ka == klee::Expr::Constant && kb == klee::Expr::Constant && a.get()->compareContents())
    */
    return false;
}

void RetChecker::initialize()
{
    bool entrySet = false;

    m_entryPoint = s2e()->getConfig()->getInt(
            getConfigKey() + ".entryPoint", 0, &entrySet);
    if (!entrySet) {
        s2e()->getWarningsStream() << "RetChecker: you must specify entryPoint" << "\n"
                                   << "You can use objdump -f $file" << "\n"
                                   << "or objdump -t $file| awk '$6 == \"main\" && NR>3 {print $1}'" << "\n";
        exit(-1);
    } else std::cerr << "Our entryPoint is " << hexval(m_entryPoint) << "\n";

    m_detector = (ModuleExecutionDetector*)s2e()->getPlugin("ModuleExecutionDetector");

    if(!m_detector)
        std::cerr << "Error loading Detector\n";

    m_hunt = s2e()->getCorePlugin()->onTranslateBlockEnd.connect(
            sigc::mem_fun(*this, &RetChecker::huntForEntryPoint));
}

void RetChecker::slotTranslateBlockEnd(ExecutionSignal *signal,
                                       S2EExecutionState* state,
                                       const ModuleDescriptor &module,
                                       TranslationBlock *tb,
                                       uint64_t endPc,
                                       bool staticTarget,
                                       uint64_t targetPc)
{
    // we'll receive onExecute signal after(!) the instruction execution
    //s2e()->getDebugStream(state) << "Translating block at"  << hexval(state->getPc()) << "\n";
    if (tb->s2e_tb_type == TB_CALL || tb->s2e_tb_type == TB_CALL_IND) {
        signal->connect(sigc::mem_fun(*this,
                                      &RetChecker::slotCall));
    } else if (tb->s2e_tb_type == TB_RET) {
        signal->connect(sigc::mem_fun(*this,
                                      &RetChecker::slotRet));
    } else {
        signal->connect(sigc::mem_fun(*this, &RetChecker::slotEveryTbEnding));
    }
}


void RetChecker::slotCall(S2EExecutionState *state, uint64_t pc)
{
    if (m_reachedEntryPoint) {
        s2e()->getDebugStream(state) << "Call executed, we're at "  << hexval(state->getPc()) << "\n";

        target_ulong newESP;
        bool ok = state->readCpuRegisterConcrete(CPU_OFFSET(regs[R_ESP]),
                                                 &newESP, sizeof(target_ulong));
        if (ok) {
            uint32_t savedRetConcrete;
            state->readMemoryConcrete(newESP, &savedRetConcrete, 4);
            klee::ref<klee::Expr> savedRet = klee::Expr::createPointer(savedRetConcrete);
            s2e()->getDebugStream(state) << "Saved return address: " << savedRet << "\n";
            s2e()->getDebugStream(state) << "And it is " << savedRet.get()->getKind() << " of bit length "
                    << savedRet.get()->getWidth() << "\n";
            DECLARE_PLUGINSTATE(RetCheckerState, state);
            plgState->storeReturnAddress(savedRet);

        } else {
            s2e()->getDebugStream(state) << "wtf? Symbolic ESP\n";
        }
    }

}

void RetChecker::slotRet(S2EExecutionState *state, uint64_t pc)
{
    if (m_reachedEntryPoint) {
        s2e()->getDebugStream(state) << "Returned to " << hexval(state->getPc()) << "\n";
        DECLARE_PLUGINSTATE(RetCheckerState, state);

        //XXX hack: we use the fact Ret is ref<Expr>
        klee::ref<klee::ConstantExpr> actualRet = klee::Expr::createPointer(state->getPc());
        plgState->checkReturnAddress(actualRet);
    }
    /*
    switch(plgState->checkReturnAddress(actualRet)) {
    case RetCheckerState::RCR_OK: s2e()->getDebugStream(state) << "RetCheck passedOk\n"; break;
    case RetCheckerState::RCR_MISMATCH: s2e()->getDebugStream(state) << "Mismatch\n"; break;
    case RetCheckerState::RCR_UNDERFLOW: s2e()->getDebugStream(state) << "Underflow\n"; break;
    */


}

void RetChecker::slotEveryTbEnding(S2EExecutionState *state, uint64_t pc)
{
    //s2e()->getDebugStream(state) << "Stepping " << hexval(state->getPc()) << "\n";

    // we assume main() is reached by instruction changing control flow
    // i.e. main() aka entryPoint starts new translation block
    // BUT! it can be run by libc and not be captured by ModuleExecutionDetector

    if (!m_reachedEntryPoint && pc == m_entryPoint) {
        s2e()->getMessagesStream(state) << "Reached entry point, enabling RetChecks";
        m_reachedEntryPoint = true;
        m_hunt.disconnect();
        m_detector->onModuleTranslateBlockEnd.connect(
                sigc::mem_fun(*this, &RetChecker::slotTranslateBlockEnd));
    }


    /*
    if (pc > m_entryPoint) {
        s2e()->getDebugStream(state) << "WTF we passed entry point " << hexval(pc) << "\n";
    }
    */
}

void RetChecker::huntForEntryPoint(
        ExecutionSignal *signal,
        S2EExecutionState* state,
        TranslationBlock *tb,
        uint64_t endPc,
        bool staticTarget,
        uint64_t targetPc)
{    
    ModuleDescriptor fake;
    slotTranslateBlockEnd(signal, state, fake, tb, endPc, staticTarget, targetPc);
}

} // namespace plugins
} // namespace s2e
