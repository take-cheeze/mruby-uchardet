mruby-uchardet
==============

mruby binding of mozilla universal charset detector.

## Original code.
Files under **src/**(except **mrb_uchardet.cxx**) is originally from [gecko-dev](https://github.com/mozilla/gecko-dev/tree/master/extensions/universalchardet).
I've modified it to build without `nscore.h` and some other dependencies.

## License
Following the original code so this code is under [Mozilla Public License 2.0](http://opensource.org/licenses/MPL-2.0).

Only copyright of **src/mrb_uchardet.cxx** is `Copyright (c) 2014 Takeshi Watanabe`.
