@echo off
pushd ..\..\plugin-tools
call build.cmd
if errorlevel 1 (
   echo !!! BUILD FAILED !!! %errorlevel%
   exit /b %errorlevel%
)
popd
yarn test
