path = "D:\repos\Test - Copy"

cd path
del *.sln

@RD /S /Q ".vs"
@RD /S /Q "Binaries"
@RD /S /Q "Build"
@RD /S /Q "DerivedDataCache"
@RD /S /Q "Intermediate"
@RD /S /Q "Saved"
