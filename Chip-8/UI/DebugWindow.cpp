#include "DebugWindow.h"

#include "Texture.h"
#include "Window.h"
#include "Chip8.h"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <imgui_internal.h>

DebugWindow::DebugWindow(Window* window, Chip8* chip)
    : _window(window), _chip(chip)
{
    Init();
}

DebugWindow::~DebugWindow()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void DebugWindow::Render(Texture* texture)
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    DockSpace();

    EmuSpace(texture);

    DebugSpace();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        GLFWwindow* backup = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup);
    }
}

void DebugWindow::Init()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

    ImGui::StyleColorsDark();

    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowBorderSize = 0.0f;
    style.WindowPadding = { 0, 0 };

    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    ImGui_ImplGlfw_InitForOpenGL(_window->GetHandle(), true);
    ImGui_ImplOpenGL3_Init("#version 460");

    ScanRoms();
}

void DebugWindow::DockSpace()
{
    ImGuiViewport* vp = ImGui::GetMainViewport();

    ImGui::SetNextWindowPos(vp->WorkPos);
    ImGui::SetNextWindowSize(vp->WorkSize);
    ImGui::SetNextWindowViewport(vp->ID);

    ImGuiWindowFlags winFlags =
        ImGuiWindowFlags_NoBackground |
        ImGuiWindowFlags_NoDecoration;

    ImGui::Begin("DockSpaceHost", nullptr, winFlags);

    ImGuiDockNodeFlags dockFlags =
        ImGuiDockNodeFlags_NoResize |
        ImGuiDockNodeFlags_NoTabBar;

    ImGuiID dockspace = ImGui::GetID("MainDockSpace");
    ImGui::DockSpace(dockspace, ImVec2(0.0f, 0.0f), dockFlags);

    bool sizeChanged = (vp->WorkSize.x != _lastSize.x || vp->WorkSize.y != _lastSize.y);

    if (_firstLoop || sizeChanged)
    {
        ImGui::DockBuilderRemoveNode(dockspace);
        ImGui::DockBuilderAddNode(dockspace, ImGuiDockNodeFlags_DockSpace);
        ImGui::DockBuilderSetNodeSize(dockspace, vp->WorkSize);

        ImGuiID left = ImGui::DockBuilderSplitNode(dockspace, ImGuiDir_Left, 0.68f, nullptr, &dockspace);
        ImGuiID right = dockspace;

        ImGui::DockBuilderDockWindow("##Emu", left);
        ImGui::DockBuilderDockWindow("##Debug", right);

        ImGui::DockBuilderFinish(dockspace);

        _firstLoop = false;
        _lastSize = vp->WorkSize;
    }

    ImGui::End();
}

void DebugWindow::EmuSpace(Texture* texture)
{
    if (ImGui::Begin("##Emu"))
    {
        const f32 texW = 64.0f;
        const f32 texH = 32.0f;

        ImVec2 avail = ImGui::GetContentRegionAvail();

        f32 sx = avail.x / texW;
        f32 sy = avail.y / texH;
        f32 scale = std::min(sx, sy);

        f32 intScale = std::floor(scale);
        if (intScale >= 1.0f)
            scale = intScale;
        else
            scale = std::max(0.0f, scale);

        ImVec2 imgSize = ImVec2(texW * scale, texH * scale);

        ImVec2 cursor = ImGui::GetCursorPos();
        ImVec2 offset = ImVec2{ std::max(0.0f, (avail.x - imgSize.x) * 0.5f), std::max(0.0f, (avail.y - imgSize.y) * 0.5f) };

        ImGui::SetCursorPos(ImVec2{ std::floor(cursor.x + offset.x), std::floor(cursor.y + offset.y) });

        ImGui::Image((ImTextureID)(intptr_t)texture->ID(), imgSize);
    }
    ImGui::End();
}

void DebugWindow::DebugSpace()
{
    if (ImGui::Begin("##Debug", nullptr, ImGuiWindowFlags_NoBackground))
    {
        ToolBar();

        RomPicker();

        DebugCPU();

        DebugStack();

        DebugDisassembly();

        DebugMemory();

        DebugKeypad();
    }

    ImGui::End();
}

void DebugWindow::DebugCPU()
{
    if (ImGui::CollapsingHeader("CPU", ImGuiTreeNodeFlags_DefaultOpen))
    {
        if (ImGui::BeginTable("cpu", 4, ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_RowBg))
        {
            ImGui::TableNextRow(); ImGui::TableNextColumn(); ImGui::TextUnformatted("PC");
            ImGui::TableNextColumn(); ImGui::Text("0x%03X", _chip->GetCPU()->GetPC());
            ImGui::TableNextColumn(); ImGui::TextUnformatted("I");
            ImGui::TableNextColumn(); ImGui::Text("0x%03X", _chip->GetCPU()->GetIndex());

            ImGui::TableNextRow(); ImGui::TableNextColumn(); ImGui::TextUnformatted("DT");
            ImGui::TableNextColumn(); ImGui::Text("%u", _chip->GetCPU()->GetDelayTimer());
            ImGui::TableNextColumn(); ImGui::TextUnformatted("ST");
            ImGui::TableNextColumn(); ImGui::Text("%u", _chip->GetCPU()->GetSoundTimer());

            for (u8 row = 0; row < 2; row++)
            {
                ImGui::TableNextRow();
                for (u8 i = 0; i < 8; i++)
                {
                    u8 r = row * 8 + i;
                    ImGui::TableNextColumn();
                    ImGui::Text("V%X", r);
                    ImGui::TableNextColumn();
                    ImGui::Text("0x%02X", _chip->GetCPU()->GetVRegister(r));
                }
            }
            ImGui::EndTable();
        }
    }

    ImGui::Separator();
}

void DebugWindow::DebugStack()
{
    if (ImGui::CollapsingHeader("Stack", ImGuiTreeNodeFlags_DefaultOpen))
    {
        const u16* st = _chip->GetCPU()->GetStack();
        const i32 sp = _chip->GetCPU()->GetSP();
        if (ImGui::BeginTable("stack", 2, ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_RowBg))
        {
            ImGui::TableSetupColumn("Idx"); ImGui::TableSetupColumn("Addr");
            ImGui::TableHeadersRow();
            for (i32 i = 0; i < 16; i++)
            {
                ImGui::TableNextRow();
                ImGui::TableNextColumn(); ImGui::Text("%2d%s", i, (i == sp ? " <- SP" : ""));
                ImGui::TableNextColumn(); ImGui::Text("0x%03X", st[i]);
            }
            ImGui::EndTable();
        }
        ImGui::Text("Depth: %d (SP=%d)", sp, sp);
    }

    ImGui::Separator();
}

void DebugWindow::DebugDisassembly()
{
    if (ImGui::CollapsingHeader("Disassembly", ImGuiTreeNodeFlags_DefaultOpen))
    {
        const u16 pc = _chip->GetCPU()->GetPC();
        const i32 linesBefore = 6;
        const i32 linesAfter = 12;

        if (ImGui::BeginTable("disasm", 3, ImGuiTableFlags_RowBg | ImGuiTableFlags_SizingFixedFit))
        {
            ImGui::TableSetupColumn("Addr");
            ImGui::TableSetupColumn("Opcode");
            ImGui::TableSetupColumn("Mnemonic");
            ImGui::TableHeadersRow();

            auto rd = [&](u16 addr)->u16 { return _chip->GetCPU()->PeekOpcode(addr); };

            const u16 start = (pc > linesBefore * 2) ? pc - linesBefore * 2 : 0;
            const u16 end = (u16)ImMin<size_t>(pc + linesAfter * 2, _chip->GetCPU()->GetMemorySize() - 2);

            for (u16 addr = start; addr <= end; addr += 2)
            {
                const u16 op = rd(addr);
                const bool atPC = (addr == pc);

                ImGui::TableNextRow();
                ImGui::TableNextColumn();

                if (atPC)
                {
                    ImGui::TableSetBgColor(ImGuiTableBgTarget_RowBg0, IM_COL32(60, 120, 200, 80));
                    ImGui::Text("> 0x%03X", addr);
                }
                else
                    ImGui::Text("  0x%03X", addr);

                ImGui::TableNextColumn(); ImGui::Text("0x%04X", op);
                ImGui::TableNextColumn(); ImGui::TextUnformatted(_chip->GetCPU()->Disassemble(addr).c_str());
            }
            ImGui::EndTable();
        }
    }

    ImGui::Separator();
}

void DebugWindow::DebugMemory()
{
    if (ImGui::CollapsingHeader("Memory (near PC & I)", ImGuiTreeNodeFlags_DefaultOpen))
    {
        const u8* mem = _chip->GetCPU()->GetMemory();
        size_t memSize = _chip->GetCPU()->GetMemorySize();

        auto hexRow = [&](u16 base)
            {
                if (base >= memSize)
                    return;

                if (ImGui::BeginTable("hex", 17, ImGuiTableFlags_SizingFixedFit))
                {
                    ImGui::TableNextRow();
                    ImGui::TableNextColumn();
                    ImGui::Text("0x%03X:", base);

                    for (u16 i = 0; i < 16; i++)
                    {
                        ImGui::TableNextColumn();
                        u16 a = base + i;
                        a < memSize ? ImGui::Text("%02X", mem[a]) : ImGui::Text("  ");
                    }

                    ImGui::EndTable();
                }
            };

        const u16 pc = _chip->GetCPU()->GetPC();
        const u16 i = _chip->GetCPU()->GetIndex();

        hexRow(pc & ~0xF);
        hexRow(i & ~0xF);
    }

    ImGui::Separator();
}

void DebugWindow::DebugKeypad()
{
    if (ImGui::CollapsingHeader("Keypad", ImGuiTreeNodeFlags_DefaultOpen))
    {
        auto isDown = [&](i32 k)->bool { return _chip->GetCPU()->IsKeyDown(k); };

        const u8 keysHex[16] =
        {
            0x1, 0x2, 0x3, 0xC,
            0x4, 0x5, 0x6, 0xD,
            0x7, 0x8, 0x9, 0xE,
            0xA, 0x0, 0xB, 0xF
        };

        const char* keys[16] =
        {
            "1","2","3","C",
            "4","5","6","D",
            "7","8","9","E",
            "A","0","B","F"
        };

        if (ImGui::BeginTable("keys", 4, ImGuiTableFlags_SizingStretchSame))
        {
            for (u8 i = 0; i < 16; i++)
            {
                if ((i % 4) == 0)
                    ImGui::TableNextRow();

                ImGui::TableNextColumn();

                bool down = _chip->GetCPU()->IsKeyDown(keysHex[i]);

                ImGui::PushStyleColor(ImGuiCol_Button, down ? ImVec4(0.25f, 0.55f, 1.0f, 1.0f) : ImVec4(0.2f, 0.2f, 0.2f, 1.0f));
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.35f, 0.65f, 1.0f, 1.0f));
                ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.15f, 0.45f, 0.9f, 1.0f));
                ImGui::Button(keys[i], ImVec2(-FLT_MIN, 0));
                ImGui::PopStyleColor(3);
            }
            ImGui::EndTable();
        }
    }

    ImGui::Separator();
}

void DebugWindow::ScanRoms()
{
    _roms.clear();

    _romDir = std::filesystem::current_path();

    for (const auto& rom : std::filesystem::directory_iterator(_romDir / "Roms"))
    {
        if (std::filesystem::is_regular_file(rom.status())
            && (rom.path().extension().string() == ".ch8" || rom.path().extension().string() == ".8o"))
        {
            _roms.push_back(rom.path());
        }
    }

    _romIndex = -1;
    for (size_t i = 0; i < _roms.size(); i++)
    {
        auto n = _roms[i].filename().string();
        if (n.find("Clock") != std::string::npos)
        {
            _romIndex = (i32)i;
            break;
        }

        if (_romIndex == -1 && !_roms.empty())
            _romIndex = 0;
    }
}

void DebugWindow::RomPicker()
{
    ImGui::TextDisabled("ROMs: %s", _romDir.empty() ? "(not found)" : _romDir.string().c_str());

    if (ImGui::Button("Rescan"))
        ScanRoms();

    ImGui::SameLine();

    if (!_roms.empty())
    {
        ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x * 0.5f);
        std::string current = (_romIndex >= 0) ? _roms[_romIndex].filename().string() : "(none)";
        if (ImGui::BeginCombo("##romcombo", current.c_str()))
        {
            for (i32 i = 0; i < (i32)_roms.size(); ++i)
            {
                bool selected = (i == _romIndex);
                if (ImGui::Selectable(_roms[i].filename().string().c_str(), selected))
                    _romIndex = i;
                if (selected) ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }

        ImGui::SameLine();
        const bool canLoad = (_romIndex >= 0);
        if (!canLoad)
            ImGui::BeginDisabled();

        if (ImGui::Button("Load"))
        {
            //_paused = true;
            _chip->Reset();
            _chip->LoadROM(_roms[_romIndex].string());
        }
        ImGui::SameLine();
        if (ImGui::Button("Reload"))
        {
            _chip->Reset();
            _chip->LoadROM(_roms[_romIndex].string());
        }

        if (!canLoad) ImGui::EndDisabled();
    }
    else
    {
        ImGui::TextUnformatted("No .ch8 files found. Put them in the 'Roms' folder next to the exe.");
    }

    ImGui::Separator();
}

void DebugWindow::ToolBar()
{
    bool paused = _chip->IsPaused();
    if (ImGui::Button(paused ? "Play" : "Pause"))
    {
        if (!_chip->GetROMSize())
        {
            ImGui::OpenPopup("NoROMLoaded");
            return;
        }

        _chip->TogglePaused();
    }

    ImGui::SetNextWindowPos(ImVec2(_window->GetWidth() / 2.0f, _window->GetHeight() / 2.0f));
    if (ImGui::BeginPopup("NoROMLoaded"))
    {
        ImGui::Text("Please load a ROM.");
        if (ImGui::Button("Close"))
        {
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }

    ImGui::SameLine();
    if (paused)
    {
        if (ImGui::Button("Step"))
            _chip->StepOnce();

        ImGui::SameLine();
    }
    else
    {
        ImGui::BeginDisabled();
        ImGui::Button("Step");
        ImGui::EndDisabled();
        ImGui::SameLine();
    }

    i32 cpf = _chip->GetCyclesPerFrame();
    ImGui::SetNextItemWidth(160);
    if (ImGui::SliderInt("Cycles/frame", &cpf, 1, 2000))
        _chip->SetCyclesPerFrame(cpf);

    if (ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows))
    {
        if (ImGui::IsKeyPressed(ImGuiKey_Space))
            _chip->TogglePaused();
        if (ImGui::IsKeyPressed(ImGuiKey_F10) && _chip->IsPaused())
            _chip->StepOnce();
    }

    ImGui::Separator();
}
