#include "Gui.h"

Gui::Gui(GLFWwindow* window, unsigned int styleColors, bool sDemoWindow, bool sAnotherWindow, ImVec4 clearColor, Rotation* pBright, Position* pModelCoords)
    : show_demo_window(sDemoWindow), show_another_window(sAnotherWindow), clear_color(clearColor), io(nullptr), bright(pBright), modelCoords(pModelCoords)
{
    
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplOpenGL3_Init();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    
    switch (styleColors)
    {
    case 0:
        ImGui::StyleColorsClassic();
        break;

    case 1:
        ImGui::StyleColorsLight();
        break;

    case 2:
        ImGui::StyleColorsDark();
        break;

    default:
        assert("No Valid style color");
        break;
    }

    io = &ImGui::GetIO();  (void)io;
    io->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io->ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
}

void Gui::guiManager()
{
    static float f = 0.0f;
    static int counter = 0;

    ImGui::Text("coordenadas: ");               // Display some text (you can use a format strings too)
    ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
    ImGui::Checkbox("Another Window", &show_another_window);

    ImGui::Text("rot: ");
    ImGui::SliderFloat3("rot", &bright->roll, -1.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f

    ImGui::Text("Coords: ");
    ImGui::SliderFloat3("coord", &modelCoords->x, 0.0f, 100.0f);

    if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
        counter++;
    ImGui::SameLine();
    ImGui::Text("counter = %d", counter);

    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io->Framerate, io->Framerate);
}

void Gui::createFrame()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void Gui::guiCreate()
{
    ImGui::Begin("Coordenadas de la rata");                          // Create a window called "Hello, world!" and append into it.

    guiManager();

    ImGui::End();
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

Gui::~Gui()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}
