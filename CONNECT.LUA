-- Script de conexión para Roblox
local function establishConnection()
    -- WebSocket para comunicación externa
    local ws = nil
    
    local function connectToExecutor()
        local success, result = pcall(function()
            -- Intentar conectar al executor
            ws = WebSocket.new("ws://localhost:8080")
            
            ws.OnMessage:Connect(function(message)
                -- Ejecutar scripts recibidos
                local success, err = pcall(function()
                    loadstring(message)()
                end)
                
                if not success then
                    warn("[Executor] Error: " .. err)
                end
            end)
            
            ws.OnClose:Connect(function()
                warn("[Executor] Conexión cerrada")
                ws = nil
                -- Reintentar conexión
                wait(5)
                connectToExecutor()
            end)
            
            print("[Executor] Conectado al executor externo")
        end)
        
        if not success then
            warn("[Executor] Error de conexión: " .. result)
            wait(10)
            connectToExecutor()
        end
    end
    
    -- Iniciar conexión
    connectToExecutor()
    
    -- Función para ejecutar scripts locales
    local function executeLocalScript(code)
        local success, result = pcall(function()
            return loadstring(code)()
        end)
        
        if success then
            return true, result
        else
            return false, result
        end
    end
    
    -- API para el executor
    local ExecutorAPI = {}
    
    function ExecutorAPI:Execute(code)
        return executeLocalScript(code)
    end
    
    function ExecutorAPI:GetPlayers()
        return game:GetService("Players"):GetPlayers()
    end
    
    function ExecutorAPI:GetLocalPlayer()
        return game:GetService("Players").LocalPlayer
    end
    
    function ExecutorAPI:GetWorkspace()
        return workspace
    end
    
    -- Exponer API globalmente
    getgenv().ExecutorAPI = ExecutorAPI
    
    print("[Executor] API cargada correctamente")
end

-- Esperar a que el juego cargue
if game:IsLoaded() then
    establishConnection()
else
    game.Loaded:Wait()
    establishConnection()
end

return "Executor Lua Module Loaded"
