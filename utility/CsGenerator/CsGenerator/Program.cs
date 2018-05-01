using CppSharp;
using CppSharp.AST;
using CppSharp.Generators;
using System;
using System.IO;

namespace CsGenerator
{
  public class Makina : ILibrary
  {
    public void Setup      (Driver driver)
    {
      driver.Options.GeneratorKind = GeneratorKind.CSharp;

      var module = driver.Options.AddModule("Makina");
      foreach (var includeDir in IncludeDirs.Split(';'))
        module.IncludeDirs.Add(includeDir);
      foreach (var include    in Includes   .Split(';'))
        module.Headers    .Add(include   );
      foreach (var libraryDir in LibraryDirs.Split(';'))
        module.LibraryDirs.Add(libraryDir);
      foreach (var library    in Libraries  .Split(';'))
        module.Libraries  .Add(library   );
    }
    public void SetupPasses(Driver driver)
    {

    }
    public void Preprocess (Driver driver, ASTContext ctx)
    {

    }
    public void Postprocess(Driver driver, ASTContext ctx)
    {

    }

    private const string IncludeDirs = "${PROJECT_SOURCE_DIR}/include;${PROJECT_INCLUDE_DIRS};${CONAN_INCLUDE_DIRS_RELEASE}";
    private const string Includes    = "${PROJECT_HPPS}";
    private const string LibraryDirs = "${PROJECT_BINARY_DIR}/Release";
    private const string Libraries   = "${PROJECT_NAME}.lib";
  }

  class Program
  {
    static void Main(string[] args)
    {
      ConsoleDriver.Run(new Makina());
    }
  }
}
