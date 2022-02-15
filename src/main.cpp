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

	class emp::Editor* editor = nullptr;
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

//#include <windows.h>
//#include <iostream>
//using namespace std;
//
//int main()
//{
//    HANDLE hIn;
//    HANDLE hOut;
//    COORD KeyWhere;
//    COORD MouseWhere;
//    COORD EndWhere;
//    bool Continue = TRUE;
//    int KeyEvents = 0;
//    int MouseEvents = 0;
//    INPUT_RECORD InRec;
//    DWORD NumRead;
//
//    hIn = GetStdHandle(STD_INPUT_HANDLE);
//    hOut = GetStdHandle(STD_OUTPUT_HANDLE);
//
//    cout << "Key Events   : " << endl;
//    cout << "Mouse Events : " << flush;
//
//    KeyWhere.X = 15;
//    KeyWhere.Y = 0;
//    MouseWhere.X = 15;
//    MouseWhere.Y = 1;
//    EndWhere.X = 0;
//    EndWhere.Y = 3;
//
//    while (Continue)
//    {
//        ReadConsoleInput(hIn,
//            &InRec,
//            1,
//            &NumRead);
//
//        switch (InRec.EventType)
//        {
//        case KEY_EVENT:
//            ++KeyEvents;
//            SetConsoleCursorPosition(hOut,
//                KeyWhere);
//            cout << KeyEvents << flush;
//            if (InRec.Event.KeyEvent.uChar.AsciiChar == 'x')
//            {
//                SetConsoleCursorPosition(hOut,
//                    EndWhere);
//                cout << "Exiting..." << endl;
//                Continue = FALSE;
//            }
//            break;
//
//        case MOUSE_EVENT:
//            ++MouseEvents;
//            SetConsoleCursorPosition(hOut,
//                MouseWhere);
//            cout << MouseEvents << flush;
//            break;
//        }
//    }
//
//    return 0;
//}
