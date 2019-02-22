//
//  Window.cpp
//  This file is part of the "Vizor Platform XCB" project and released under the .
//
//  Created by Huba Nagy on 22/2/2019.
//  Copyright, 2019, by Huba Nagy. All rights reserved.
//

#include <UnitTest/UnitTest.hpp>

#include <Vizor/Platform/XCB/Window.hpp>

#include <Time/Interval.hpp>

namespace Vizor
{
	namespace Platform
	{
		namespace XCB
		{
			UnitTest::Suite WindowTestSuite {
				"Vizor::Platform::XCB::Window",
				
				{"it should be able to show a window for a few seconds",
					[](UnitTest::Examiner & examiner) {
						Window window;

						window.set_title("doot!");
						window.show_window();

						Time::Interval(3).sleep();

						window.hide_window();
					}
				},
			};
		}
	}
}
