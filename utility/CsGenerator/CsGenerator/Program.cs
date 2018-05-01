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

      var splitIncludeDirs = IncludeDirs.Split(';');
      var splitIncludes    = Includes   .Split(';');
      var splitLibraryDirs = LibraryDirs.Split(';');
      var splitLibraries   = Libraries  .Split(';');

      foreach (var includeDir in splitIncludeDirs)
        module.IncludeDirs.Add(includeDir);
      foreach (var include    in splitIncludes   )
        module.Headers    .Add(include   );
      foreach (var libraryDir in splitLibraryDirs)
        module.LibraryDirs.Add(libraryDir);
      foreach (var library    in splitLibraries)
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

    private const string IncludeDirs = "${PROJECT_SOURCE_DIR}/include;${PROJECT_INCLUDE_DIRS}";
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
