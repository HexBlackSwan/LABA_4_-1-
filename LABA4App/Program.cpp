#include "MainForm.h"

using namespace System;
using namespace System::Windows::Forms;
using namespace Laba4App;

[STAThreadAttribute]
int main(array<String^>^ args) {
    Application::EnableVisualStyles();
    Application::SetCompatibleTextRenderingDefault(false);
    Application::Run(gcnew MainForm());
    return 0;
}
