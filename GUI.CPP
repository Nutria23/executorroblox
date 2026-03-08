#include <Windows.h>
#include <commctrl.h>
#include <string>
#include <vector>

#pragma comment(lib, "comctl32.lib")

#define ID_BUTTON_EXECUTE 1001
#define ID_BUTTON_ATTACH 1002
#define ID_EDIT_SCRIPT 1003
#define ID_COMBO_TEMPLATES 1004
#define ID_CHECK_ANTIBAN 1005
#define ID_CHECK_OBFUSCATE 1006

HWND hEditScript, hButtonExecute, hButtonAttach, hComboTemplates;
HWND hCheckAntiBan, hCheckObfuscate;
AdvancedRobloxInjector* injector = nullptr;

// Templates de scripts
std::vector<std::pair<std::string, std::string>> scriptTemplates = {
    {"Aimbot", R"(
        local Players = game:GetService("Players")
        local RunService = game:GetService("RunService")
        local UserInputService = game:GetService("UserInputService")
        
        local LocalPlayer = Players.LocalPlayer
        local Mouse = LocalPlayer:GetMouse()
        
        local aimbotEnabled = false
        local smoothing = 0.1
        local fov = 100
        
        local function getClosestPlayer()
            local closest = nil
            local shortestDistance = math.huge
            
            for _, player in pairs(Players:GetPlayers()) do
                if player ~= LocalPlayer and player.Character then
                    local humanoid = player.Character:FindFirstChild("Humanoid")
                    local head = player.Character:FindFirstChild("Head")
                    
                    if humanoid and humanoid.Health > 0 and head then
                        local distance = (head.Position - LocalPlayer.Character.Head.Position).Magnitude
                        if distance < shortestDistance and distance < fov then
                            shortestDistance = distance
                            closest = player
                        end
                    end
                end
            end
            
            return closest
        end
        
        UserInputService.InputBegan:Connect(function(input)
            if input.KeyCode == Enum.KeyCode.LeftAlt then
                aimbotEnabled = not aimbotEnabled
                print("Aimbot: " .. (aimbotEnabled and "ON" or "OFF"))
            end
        end)
        
        RunService.RenderStepped:Connect(function()
            if aimbotEnabled then
                local target = getClosestPlayer()
                if target and target.Character and target.Character:FindFirstChild("Head") then
                    local camera = workspace.CurrentCamera
                    local targetPos = target.Character.Head.Position
                    
                    -- Suavizado
                    local current = camera.CFrame
                    local lookVector = (targetPos - current.Position).Unit
                    local goal = CFrame.new(current.Position, current.Position + lookVector)
                    
                    camera.CFrame = current:Lerp(goal, smoothing)
                end
            end
        end)
    )"},
    
    {"ESP", R"(
        local Players = game:GetService("Players")
        local RunService = game:GetService("RunService")
        
        local LocalPlayer = Players.LocalPlayer
        local espFolder = Instance.new("Folder")
        espFolder.Name = "ESP_Folder"
        espFolder.Parent = game.CoreGui
        
        local function createESP(player)
            if player == LocalPlayer then return end
            
            local highlight = Instance.new("Highlight")
            highlight.Name = player.Name .. "_ESP"
            highlight.Adornee = nil
            highlight.FillColor = Color3.fromRGB(255, 0, 0)
            highlight.OutlineColor = Color3.fromRGB(255, 255, 0)
            highlight.FillTransparency = 0.7
            highlight.Parent = espFolder
            
            local billboard = Instance.new("BillboardGui")
            billboard.Name = player.Name .. "_Info"
            billboard.Size = UDim2.new(0, 200, 0, 100)
            billboard.StudsOffset = Vector3.new(0, 3, 0)
            billboard.AlwaysOnTop = true
            billboard.Parent = espFolder
            
            local nameLabel = Instance.new("TextLabel")
            nameLabel.Text = player.Name
            nameLabel.TextColor3 = Color3.fromRGB(255, 255, 255)
            nameLabel.TextStrokeColor3 = Color3.fromRGB(0, 0, 0)
            nameLabel.TextStrokeTransparency = 0
            nameLabel.BackgroundTransparency = 1
            nameLabel.Size = UDim2.new(1, 0, 0.5, 0)
            nameLabel.Parent = billboard
            
            local healthLabel = Instance.new("TextLabel")
            healthLabel.Text = "HP: 100/100"
            healthLabel.TextColor3 = Color3.fromRGB(0, 255, 0)
            healthLabel.TextStrokeTransparency = 0
            healthLabel.BackgroundTransparency = 1
            healthLabel.Size = UDim2.new(1, 0, 0.5, 0)
            healthLabel.Position = UDim2.new(0, 0, 0.5, 0)
            healthLabel.Parent = billboard
            
            RunService.Heartbeat:Connect(function()
                if player.Character and player.Character:FindFirstChild("Humanoid") then
                    local humanoid = player.Character.Humanoid
                    local rootPart = player.Character:FindFirstChild("HumanoidRootPart")
                    
                    if rootPart then
                        highlight.Adornee = rootPart
                        billboard.Adornee = rootPart
                        
                        -- Actualizar salud
                        healthLabel.Text = string.format("HP: %d/%d", 
                            math.floor(humanoid.Health), 
                            math.floor(humanoid.MaxHealth))
                        
                        -- Cambiar color según salud
                        local healthPercent = humanoid.Health / humanoid.MaxHealth
                        highlight.FillColor = Color3.fromRGB(
                            255 * (1 - healthPercent),
                            255 * healthPercent,
                            0
                        )
                        
                        -- Distancia
                        if LocalPlayer.Character and LocalPlayer.Character:FindFirstChild("HumanoidRootPart") then
                            local distance = (rootPart.Position - LocalPlayer.Character.HumanoidRootPart.Position).Magnitude
                            nameLabel.Text = string.format("%s [%dm]", player.Name, math.floor(distance))
                        end
                    end
                else
                    highlight.Adornee = nil
                    billboard.Adornee = nil
                end
            end)
        end
        
        for _, player in pairs(Players:GetPlayers()) do
            createESP(player)
        end
        
        Players.PlayerAdded:Connect(createESP)
    )"},
    
    {"Fly Hack", R"(
        local Players = game:GetService("Players")
        local UserInputService = game:GetService("UserInputService")
        local RunService = game:GetService("RunService")
        
        local LocalPlayer = Players.LocalPlayer
        local Character = LocalPlayer.Character or LocalPlayer.CharacterAdded:Wait()
        local Humanoid = Character:WaitForChild("Humanoid")
        local RootPart = Character:WaitForChild("HumanoidRootPart")
        
        local flying = false
        local flySpeed = 50
        local bodyVelocity
        
        local function startFlying()
            if flying or not RootPart then return end
            
            flying = true
            
            bodyVelocity = Instance.new("BodyVelocity")
            bodyVelocity.Velocity = Vector3.new(0, 0, 0)
            bodyVelocity.MaxForce = Vector3.new(100000, 100000, 100000)
            bodyVelocity.P = 10000
            bodyVelocity.Parent = RootPart
            
            local bodyGyro = Instance.new("BodyGyro")
            bodyGyro.MaxTorque = Vector3.new(100000, 100000, 100000)
            bodyGyro.P = 10000
            bodyGyro.D = 100
            bodyGyro.Parent = RootPart
            
            Humanoid.PlatformStand = true
            
            print("Fly Hack: ON (F para alternar)")
        end
        
        local function stopFlying()
            if not flying then return end
            
            flying = false
            
            if bodyVelocity then
                bodyVelocity:Destroy()
                bodyVelocity = nil
            end
            
            Humanoid.PlatformStand = false
            
            print("Fly Hack: OFF")
        end
        
        UserInputService.InputBegan:Connect(function(input)
            if input.KeyCode == Enum.KeyCode.F then
                if flying then
                    stopFlying()
                else
                    startFlying()
                end
            end
        end)
        
        RunService.Heartbeat:Connect(function()
            if not flying or not bodyVelocity or not RootPart then return end
            
            local direction = Vector3.new(0, 0, 0)
            
            if UserInputService:IsKeyDown(Enum.KeyCode.W) then
                direction = direction + RootPart.CFrame.LookVector
            end
            if UserInputService:IsKeyDown(Enum.KeyCode.S) then
                direction = direction - RootPart.CFrame.LookVector
            end
            if UserInputService:IsKeyDown(Enum.KeyCode.A) then
                direction = direction - RootPart.CFrame.RightVector
            end
            if UserInputService:IsKeyDown(Enum.KeyCode.D) then
                direction = direction + RootPart.CFrame.RightVector
            end
            if UserInputService:IsKeyDown(Enum.KeyCode.Space) then
                direction = direction + Vector3.new(0, 1, 0)
            end
            if UserInputService:IsKeyDown(Enum.KeyCode.LeftShift) then
                direction = direction + Vector3.new(0, -1, 0)
            end
            
            if direction.Magnitude > 0 then
                direction = direction.Unit * flySpeed
            end
            
            bodyVelocity.Velocity = direction
        end)
    )"}
};

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_CREATE: {
        // Crear controles
        hEditScript = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", "",
            WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_MULTILINE | ES_AUTOVSCROLL,
            10, 50, 600, 300, hwnd, (HMENU)ID_EDIT_SCRIPT, NULL, NULL);
        
        hButtonExecute = CreateWindow("BUTTON", "EJECUTAR SCRIPT",
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            10, 360, 150, 30, hwnd, (HMENU)ID_BUTTON_EXECUTE, NULL, NULL);
        
        hButtonAttach = CreateWindow("BUTTON", "CONECTAR A ROBLOX",
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            170, 360, 150, 30, hwnd, (HMENU)ID_BUTTON_ATTACH, NULL, NULL);
        
        hComboTemplates = CreateWindow("COMBOBOX", "",
            WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST,
            10, 10, 200, 200, hwnd, (HMENU)ID_COMBO_TEMPLATES, NULL, NULL);
        
        hCheckAntiBan = CreateWindow("BUTTON", "Anti-Ban System",
            WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX,
            350, 10, 150, 20, hwnd, (HMENU)ID_CHECK_ANTIBAN, NULL, NULL);
        
        hCheckObfuscate = CreateWindow("BUTTON", "Ofuscar Código",
            WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX,
            350, 35, 150, 20, hwnd, (HMENU)ID_CHECK_OBFUSCATE, NULL, NULL);
        
        // Configurar combo box
        for (const auto& template : scriptTemplates) {
            SendMessage(hComboTemplates, CB_ADDSTRING, 0, (LPARAM)template.first.c_str());
        }
        
        // Configurar font
        HFONT hFont = CreateFont(14, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
                                DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                                DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Consolas");
        SendMessage(hEditScript, WM_SETFONT, (WPARAM)hFont, TRUE);
        
        // Establecer texto inicial
        SetWindowText(hEditScript, "-- Escribe tu script Lua aquí...\n-- O selecciona una plantilla\n\nprint('Roblox Universal Executor')");
        
        break;
    }
    
    case WM_COMMAND: {
        int wmId = LOWORD(wParam);
        
        if (wmId == ID_BUTTON_EXECUTE) {
            char scriptText[65536];
            GetWindowText(hEditScript, scriptText, 65536);
            
            if (injector) {
                injector->ExecuteScript(scriptText);
            } else {
                MessageBox(hwnd, "Primero conecta a Roblox", "Error", MB_ICONERROR);
            }
        }
        else if (wmId == ID_BUTTON_ATTACH) {
            if (!injector) {
                injector = new AdvancedRobloxInjector();
            }
            
            if (injector->Initialize()) {
                MessageBox(hwnd, "Conectado a Roblox exitosamente", "Éxito", MB_ICONINFORMATION);
            } else {
                MessageBox(hwnd, "No se pudo conectar a Roblox", "Error", MB_ICONERROR);
            }
        }
        else if (wmId == ID_COMBO_TEMPLATES) {
            if (HIWORD(wParam) == CBN_SELCHANGE) {
                int index = SendMessage(hComboTemplates, CB_GETCURSEL, 0, 0);
                if (index >= 0 && index < scriptTemplates.size()) {
                    SetWindowText(hEditScript, scriptTemplates[index].second.c_str());
                }
            }
        }
        
        break;
    }
    
    case WM_DESTROY:
        if (injector) {
            injector->Cleanup();
            delete injector;
        }
        PostQuitMessage(0);
        break;
    
    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    // Registrar clase de ventana
    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = "RobloxExecutorClass";
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    
    RegisterClass(&wc);
    
    // Crear ventana
    HWND hwnd = CreateWindowEx(0, "RobloxExecutorClass", "Roblox Universal Executor v2.0",
                               WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX,
                               CW_USEDEFAULT, CW_USEDEFAULT, 650, 450,
                               NULL, NULL, hInstance, NULL);
    
    if (!hwnd) return 0;
    
    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);
    
    // Loop de mensajes
    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    
    return 0;
}
