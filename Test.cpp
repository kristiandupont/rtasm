//[of]:Copyright statement
//[c](C) Copyright 2005, 2012 Kristian Dupont Knudsen. MIT license -- see LICENSE.md for details
//[c]This file is most easily read with Code Browser: http://tibleiz.net/code-browser/
//[cf]
#include "stdafx.h"

#include <cppunit/CompilerOutputter.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>

using namespace std;

int RunTests()
{
	printf("Running tests\n");
	// Get the top level suite from the registry
	CppUnit::Test* suite = CppUnit::TestFactoryRegistry::getRegistry().makeTest();

	// Adds the test to the list of test to run
	CppUnit::TextUi::TestRunner runner;
	runner.addTest(suite);

	// Change the default outputter to a compiler error format outputter
	runner.setOutputter(new CppUnit::CompilerOutputter(&runner.result(), std::cerr));
	// Run the tests.
	bool wasSucessful = runner.run();

	// Return error code 1 if the one of test failed.
	return wasSucessful ? 0 : 1;
}

int _tmain(int argc, _TCHAR* argv[])
{
	if(argc == 2)
	{
		if(string(argv[1]) == "/T")
			return RunTests();
	}

	return 0;
}

