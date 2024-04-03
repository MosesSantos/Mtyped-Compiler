:: COMPILERS COURSE - SCRIPT ---------------------------------------------
:: SCRIPT A32 - CST8152 - Winter 2022

CLS
SET COMPILER=Compiler.exe
SET LOCATION=.\InputFiles

SET FILE1=%LOCATION%\MTYPED_Empty
SET FILE2=%LOCATION%\MTYPED_Hello
SET FILE3=%LOCATION%\MTYPED_Volume

SET EXTENSION=mty
SET OUTPUT=out
SET ERROR=err

SET PARAM=3

:: ---------------------------------------------------------------------
:: Begin of Tests (A32 - W22) ------------------------------------------
:: ---------------------------------------------------------------------

ren *.exe %COMPILER%

::
:: BASIC TESTS  ----------------------------------------------------------
::
:: Basic Tests (A32 - W22) - - - - - - - - - - - - - - - - - - - - - -

%COMPILER% %PARAM% %FILE1%.%EXTENSION%	> %FILE1%.%OUTPUT%	2> %FILE1%.%ERROR%
%COMPILER% %PARAM% %FILE2%.%EXTENSION%	> %FILE2%.%OUTPUT%	2> %FILE2%.%ERROR%
%COMPILER% %PARAM% %FILE3%.%EXTENSION%	> %FILE3%.%OUTPUT%	2> %FILE3%.%ERROR%

:: SHOW OUTPUTS - - - - - - - - - - - - - - - - - - - - - - - - - - -
DIR *.OUT
DIR *.ERR

:: ---------------------------------------------------------------------
:: End of Tests (A32 - W22) --------------------------------------------
:: ---------------------------------------------------------------------
