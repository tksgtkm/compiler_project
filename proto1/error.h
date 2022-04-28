#ifndef __ERROR_H
#define __ERROR_H

extern int errorCount;

enum TAbortCode {
  abortInvalidCommandLineArgs = -1;
  abortourceFileOpenFailed    = -2;
  abortIFormFileOpenFailed    = -3;
  abortAssemblyFileOpenFailed = -4;
  abortTooManySyntaxErrors    = -5;
  abortStackOverflow          = -6;
  abortCodeSegmentOverflow    = -7;
  abortNestingToodeep         = -8;
  abortRuntimeError           = -9;
  abortUnimplementedFeature   = -10;
};

void AbortTranslation(TAbortCode ac);

#endif
