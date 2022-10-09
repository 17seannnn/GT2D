----------------------------------------------------------------------
--| * GarageBlueprint.lua *
--|
--| Contains all stuff for garage missions
----------------------------------------------------------------------

---- Includes
dofile "Scripts/MissionDefines.lua"

---- Defines
ACTOR_TEAM_FRIENDS = ACTOR_TEAM_DEFAULT + 1

---- Resources
Textures["Background"] = defineTexture("Textures/Locations/Mission0-1.png", TW_LOCATION, TH_LOCATION)
Textures["Player"] = defineTexture("Textures/Actors/Player.png", TW_ACTOR, TH_ACTOR)
Textures["Zhenek"] = defineTexture("Textures/Actors/Zhenek.png", TW_ACTOR, TH_ACTOR)
Textures["Anthony"] = defineTexture("Textures/Actors/Anthony.png", TW_ACTOR, TH_ACTOR)
Textures["PlaceholderCar"] = defineTexture("Textures/Cars/TrashCar.png", TW_CAR, TH_CAR)

Anims["SlowMoving"] = defineAnimation(1, 5, 1000.0 / 13.5)
Anims["RepairCar"] = defineAnimation(4, 2, 1000.0 / 1)
Anims["TakeInstruments"] = defineAnimation(5, 2, 1000.0 / 1)

States["UpCar"] = defineState("stateUpCar")
States["TakeInstruments"] = defineState("stateTakeInstruments")
States["RandomTalk"] = defineState("stateRandomTalk")
States["RepairCar"] = defineState("stateRepairCar")

---- Globals
GROUND_WIDTH = GW_LOCATION
GROUND_HEIGHT = 18
GROUND_X = 0
GROUND_Y = GH_LOCATION - GROUND_HEIGHT

Zhenek = nil
Anthony = nil
Car = nil

IsZhenekBusy = false -- Change this when you cut-scene require Zhenek

---- Required functions
function onGarageEnter()
    -- Entities
    Player = addActor(GW_LOCATION - GW_ACTOR, GH_LOCATION - GH_ACTOR, GW_ACTOR, GH_ACTOR, Textures["Player"])
    local XDefault,YDefault = getActorSpeed(Player)
    setActorSpeed(Player, XDefault/1.75, YDefault/2)
    setActorAnim(Player, ACTOR_ANIMATION_HORIZONTAL, Anims["SlowMoving"])
    setActorTeam(Player, ACTOR_TEAM_FRIENDS)
    setActorWeapon(Player, Weapons["Fist"])

    Zhenek = addActor(85, 47, GW_ACTOR, GH_ACTOR, Textures["Zhenek"])
    setActorSpeed(Zhenek, XDefault/2, YDefault/2)
    setActorAnim(Zhenek, ACTOR_ANIMATION_HORIZONTAL, Anims["SlowMoving"])
    setActorTeam(Zhenek, ACTOR_TEAM_FRIENDS)
    turnActorLeft(Zhenek)

    Anthony = addActor(50, 50, GW_ACTOR, GH_ACTOR, Textures["Anthony"])
    setActorSpeed(Anthony, XDefault/2, YDefault/2)
    setActorAnim(Anthony, ACTOR_ANIMATION_HORIZONTAL, Anims["SlowMoving"])
    setActorTeam(Anthony, ACTOR_TEAM_FRIENDS)
    setActorState(Anthony, States["UpCar"])
    turnActorLeft(Anthony)

    Car = addCar(25, 50, 65, 20, Textures["PlaceholderCar"])
    setEntityRenderMode(Car, RENDER_MODE_FOREGROUND)
    setEntityZIndex(Car, -1)
    setCarMaxSpeed(Car, 0, 0.005)
    setCarAcceleration(Car, 0, -0.000001)

    -- Cutscenes
    TakeInstruments = {
        { Anthony, true, GTT_GOTO, 63, 47.5 },
        { Anthony, true, GTT_ANIMATE_FOR, Anims["TakeInstruments"], math.random(2000, 5000) },
        { Anthony, true, GTT_PUSH_COMMAND, GTC_MOVE_IDLE },
        { Anthony, true, GTT_WAIT, 500.0 },
    }
    TakeInstrumentsStage = 0

    RepairCar = {
        { Anthony, true, GTT_GOTO, 25, 53 },
        { Anthony, true, GTT_PUSH_COMMAND, GTC_TURN_RIGHT },
        { Anthony, true, GTT_ANIMATE_FOR, Anims["RepairCar"], math.random(5000, 15000) },
        { Anthony, true, GTT_PUSH_COMMAND, GTC_MOVE_IDLE },
        { Anthony, true, GTT_WAIT, 500.0 },
    }
    RepairCarStage = 0

    RandomTalk = {}
    RandomTalkStage = 0

    -- Ground
    setGroundBounds(GROUND_X, GROUND_Y, GROUND_WIDTH, GROUND_HEIGHT)

    -- Camera
    setCameraBounds(0, 0, GW_LOCATION, GH_LOCATION)
    setCameraPosition(0, 0)
end

function onGarageRender()
	drawFrame(RENDER_MODE_BACKGROUND, 0, false, 0,0,SCREEN_WIDTH,SCREEN_HEIGHT, Textures["Background"], 0, 0)
end

---- Internal functions
function stateUpCar(Actor)
    local X,Y = getEntityPosition(Car)

    if Y >= 44 and Y <= 45 then
        setCarAcceleration(Car, 0, 0.0000003)
    elseif Y <= 40 then
        setEntityPosition(Car, 25, 40)
        setCarMaxSpeed(Car, 0, 0)
        setActorState(Actor, States["TakeInstruments"])
    end
end

stateTakeInstruments = createCutscene(
    function()
        return TakeInstruments
    end,
    function(Change)
        TakeInstrumentsStage = TakeInstrumentsStage + Change
        return TakeInstrumentsStage
    end,
    function(Actor)
    end,
    function(Actor)
        TakeInstruments[2][5] = math.random(2000, 5000)
        TakeInstrumentsStage = 0

        if not IsZhenekBusy and math.random(0, 1) == 1 then
            setActorState(Actor, States["RandomTalk"])
        else
            setActorState(Actor, States["RepairCar"])
        end
    end
)

stateRepairCar = createCutscene(
    function()
        return RepairCar
    end,
    function(Change)
        RepairCarStage = RepairCarStage + Change
        return RepairCarStage
    end,
    function(Actor)
    end,
    function(Actor)
        RepairCar[2][5] = math.random(5000, 15000) -- Wait time
        RepairCarStage = 0
        setActorState(Actor, States["TakeInstruments"])
    end
)

stateRandomTalk = createCutscene(
    function()
        return RandomTalk
    end,
    function(Change)
        RandomTalkStage = RandomTalkStage + Change
        return RandomTalkStage
    end,
    function(Actor)
        local Res = math.random(1, 2)
        if Res == 1 then
            RandomTalk = {
                { Anthony, true, GTT_WAIT_DIALOG, addDialog(GW_DIALOG, GH_DIALOG, "Sometimes i want to change my job", 4, Anthony, Textures["DialogSquare"]) },
                { Zhenek, true, GTT_WAIT_DIALOG, addDialog(GW_DIALOG, GH_DIALOG, "Your problems.", 1.5, Zhenek, Textures["DialogSquare"]) },
                { Anthony, true, GTT_WAIT, 1000 }
            }
        elseif Res == 2 then
            RandomTalk = {
                { Anthony, true, GTT_WAIT_DIALOG, addDialog(GW_DIALOG, GH_DIALOG, "How you doing, bruh?", 3, Anthony, Textures["DialogSquare"]) },
                { Zhenek, true, GTT_WAIT_DIALOG, addDialog(GW_DIALOG, GH_DIALOG, "Can..", 0.5, Zhenek, Textures["DialogSquare"]) },
                { Zhenek, true, GTT_WAIT_DIALOG, addDialog(GW_DIALOG, GH_DIALOG, "You..", 0.5, Zhenek, Textures["DialogSquare"]) },
                { Zhenek, true, GTT_WAIT_DIALOG, addDialog(GW_DIALOG, GH_DIALOG, "PhIl MaHaAar", 2.5, Zhenek, Textures["DialogSquare"]) },
                { Anthony, true, GTT_WAIT, 1000 }
            }
        end
    end,
    function(Actor)
        RandomTalkStage = 0
        setActorState(Actor, States["RepairCar"])
    end
)
