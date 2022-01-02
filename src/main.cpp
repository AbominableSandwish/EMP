#include <iostream>
#include <string>
#include <core/engine.h>
#include <core/config.h>

#include "editor/editor.h"


int main() {
	emp::Mode mode = emp::Mode::Editor;
	
    emp::ConfigEngine* config = new emp::ConfigEngine("Mushroom Engine", mode);
    emp::Engine* engine = new emp::Engine();
    engine->Init(config);

	class emp::Editor* editor = nullptr; // todo GET OUT!
	if (mode == emp::Mode::Editor)
	{		
		editor = new class emp::Editor(*engine);
		editor->Init();
	}
	
    engine->Start();
    
	while (engine->is_running)
	{
		editor->Draw();
		if(editor != nullptr)
			engine->Update();
	}

    if (editor != nullptr) {
        editor->Destroy();
        editor = nullptr;
    }
   
    exit(EXIT_SUCCESS);
}
