@echo off
msbuild /t:Build /m /p:BuildInParallel=true /fl /verbosity:n Dunjun.sln
