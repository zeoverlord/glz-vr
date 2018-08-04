// Copyright 2013 Peter Wallstr�m
//
// Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License. You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0

// Unless required by applicable law or agreed to in writing, software distributed under the License is
// distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and limitations under the License.

// while it is not required i like to request a few things
// 1. please do share any meaningfull/usefull changes/additions/fixes you make with me so that i could include it in any future version
// 2. likewise do share any ideas for improvements
// 3. If you make something comersiol or at least something you release publicly that relies on this code then i would like to know and maybe use in my CV
// 4. Please do include me in your credits

// blank example app - i wouldn't say it produces any meningfull results, but it's a start.
// visit http://www.flashbang.se or contact me at overlord@flashbang.se
// the entire toolkit should exist in it's entirety at github
// https://github.com/zeoverlord/glz.git



#include "..\..\glz-basecode\win\zeobase2.h"
#include "..\..\glz-core\app\appbase.h"
#include "..\..\glz-core\state\baseState.h"
#include "..\..\glz-core\state\stateManager.h"
#include "Example_vr.h"

void preInitialize(void)
{
	GLZ::glzStateManager stateManager;
	std::shared_ptr<GLZ::glzBaseState> gameState(new ExampleVRState);
	stateManager.addState(gameState, "mainstate");
	stateManager.switchState("mainstate");
	GLZ::glzAppinitialization app;
	app.set_title(L"ZeoBase GL Framework");
	app.data.ALLOW_RESIZE = true;
	return;
}
