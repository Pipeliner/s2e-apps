#!/bin/sh
# Answer is 'test'
./assert '[ "098f6bcd4621d373cade4e832627b4f6  -" = "$(echo -n '$1' | md5sum)" ]'
