# Input Configuration Guide

This document describes the required input bindings for the Voxel Survival game.

## Setup Instructions

1. Open Unreal Engine Editor
2. Go to Edit → Project Settings → Input
3. Add the following Action Mappings and Axis Mappings

## Action Mappings

Action mappings trigger once when pressed or released.

### Movement Actions
| Action Name | Key | Description |
|------------|-----|-------------|
| Jump | Space Bar | Character jump |
| Jump | Gamepad Face Button Bottom | Controller jump |
| Sprint | Left Shift | Sprint (hold) |
| Sprint | Gamepad Left Thumbstick | Controller sprint |

### Interaction Actions
| Action Name | Key | Description |
|------------|-----|-------------|
| Use | Left Mouse Button | Use equipped tool/weapon |
| Use | Gamepad Right Trigger | Controller use item |
| Interact | E | Interact with objects |
| Interact | Gamepad Face Button Right | Controller interact |
| Inventory | Tab | Toggle inventory UI |
| Inventory | Gamepad Special Right | Controller inventory |

### Hotbar Actions
| Action Name | Key | Description |
|------------|-----|-------------|
| Hotbar1 | 1 | Select hotbar slot 1 |
| Hotbar2 | 2 | Select hotbar slot 2 |
| Hotbar3 | 3 | Select hotbar slot 3 |
| Hotbar4 | 4 | Select hotbar slot 4 |
| Hotbar5 | 5 | Select hotbar slot 5 |
| Hotbar6 | 6 | Select hotbar slot 6 |
| Hotbar7 | 7 | Select hotbar slot 7 |
| Hotbar8 | 8 | Select hotbar slot 8 |
| Hotbar9 | 9 | Select hotbar slot 9 |
| Hotbar0 | 0 | Select hotbar slot 10 |

## Axis Mappings

Axis mappings provide continuous input values.

### Movement Axes
| Axis Name | Key | Scale | Description |
|-----------|-----|-------|-------------|
| MoveForward | W | 1.0 | Move forward |
| MoveForward | S | -1.0 | Move backward |
| MoveForward | Gamepad Left Thumbstick Up/Down | 1.0 | Controller forward/back |
| MoveRight | D | 1.0 | Move right |
| MoveRight | A | -1.0 | Move left |
| MoveRight | Gamepad Left Thumbstick Left/Right | 1.0 | Controller left/right |

### Camera Axes
| Axis Name | Key | Scale | Description |
|-----------|-----|-------|-------------|
| Turn | Mouse X | 1.0 | Camera horizontal rotation |
| Turn | Gamepad Right Thumbstick Left/Right | 1.0 | Controller camera rotation |
| LookUp | Mouse Y | -1.0 | Camera vertical rotation (inverted) |
| LookUp | Gamepad Right Thumbstick Up/Down | 1.0 | Controller look up/down |

## Console Variables (Optional)

These can be set in DefaultInput.ini for fine-tuning:

```ini
[/Script/Engine.InputSettings]
; Mouse sensitivity
MouseSensitivity=0.5

; Controller dead zones
GamepadLeftStickDeadZone=0.25
GamepadRightStickDeadZone=0.25

; Invert controls
bInvertY=False
bInvertX=False
```

## Enhanced Input System (Alternative)

If using Unreal Engine 5's Enhanced Input system:

### Input Mapping Context: IMC_Default

#### Input Actions

**IA_Move** (Value Type: Axis2D)
- W/S/A/D → Vector2D movement
- Left Thumbstick → Vector2D movement

**IA_Look** (Value Type: Axis2D)
- Mouse X/Y → Vector2D camera rotation
- Right Thumbstick → Vector2D camera rotation

**IA_Jump** (Value Type: Digital)
- Space Bar → Pressed/Released
- Face Button Bottom → Pressed/Released

**IA_Sprint** (Value Type: Digital)
- Left Shift → Hold
- Left Thumbstick Button → Hold

**IA_Use** (Value Type: Digital)
- Left Mouse Button → Pressed
- Right Trigger → Pressed

**IA_Interact** (Value Type: Digital)
- E → Pressed
- Face Button Right → Pressed

**IA_Inventory** (Value Type: Digital)
- Tab → Toggle
- Special Right → Toggle

**IA_Hotbar** (Value Type: Digital, Repeated)
- 1-9, 0 → Individual actions for each slot

### Modifiers

- **Look**: Dead Zone (0.25), Smooth (0.1)
- **Move**: Dead Zone (0.25)
- **Sprint**: Hold (0.3 seconds)

### Triggers

- **Use**: Pressed (for instant action)
- **Jump**: Pressed/Released (for hold time)
- **Sprint**: Hold (continuous)
- **Interact**: Pressed (single fire)

## Testing Input

After configuration, test the following:

1. **Movement**
   - WASD should move character
   - Mouse should rotate camera
   - Shift should increase speed

2. **Actions**
   - Space should make character jump
   - E should highlight interactable objects
   - Left click should use equipped tool

3. **Hotbar**
   - Number keys 1-0 should select slots
   - Selected slot should highlight in UI
   - Tool should appear in hand

4. **Inventory**
   - Tab should open/close inventory
   - Mouse should work in inventory UI
   - Drag and drop should work

## Troubleshooting

### Input Not Working
- Verify input mode is set to "Game and UI" or "Game Only"
- Check that player controller has input enabled
- Ensure input component is properly bound

### Double Input
- Check for duplicate bindings in project settings
- Verify input consumption (StopAction) is used where needed

### Controller Not Responding
- Enable gamepad in project settings
- Check controller is detected in Windows/OS
- Verify dead zones are not too high

### Mouse Sensitivity Issues
- Adjust Turn/LookUp scale values
- Add mouse sensitivity multiplier in code
- Check DPI settings in OS

## Platform-Specific Notes

### PC
- All keyboard and mouse inputs work by default
- No special configuration needed

### Console (Xbox/PlayStation)
- Gamepad bindings are required
- Face buttons may be different (A/B vs X/O)
- Test on target platform

### Mobile
- Touch controls need separate implementation
- Virtual joysticks recommended
- UI buttons for actions

## Blueprint Integration

To use these inputs in Blueprint:

1. Right-click in Event Graph
2. Search for action name (e.g., "Jump")
3. Connect to desired functionality

Example: Jump Action
```
Event Jump (Pressed) → Jump (Character)
Event Jump (Released) → Stop Jumping (Character)
```

Example: Hotbar Selection
```
Event Hotbar1 → Select Hotbar Slot (0)
Event Hotbar2 → Select Hotbar Slot (1)
...etc
```

## C++ Integration

Input bindings in SurvivalPlayerCharacter::SetupPlayerInputComponent():

```cpp
// Movement axes
PlayerInputComponent->BindAxis("MoveForward", this, &ASurvivalPlayerCharacter::MoveForward);
PlayerInputComponent->BindAxis("MoveRight", this, &ASurvivalPlayerCharacter::MoveRight);
PlayerInputComponent->BindAxis("Turn", this, &ASurvivalPlayerCharacter::Turn);
PlayerInputComponent->BindAxis("LookUp", this, &ASurvivalPlayerCharacter::LookUp);

// Action bindings
PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ASurvivalPlayerCharacter::StartJump);
PlayerInputComponent->BindAction("Use", IE_Pressed, this, &ASurvivalPlayerCharacter::UseEquippedItem);
```

This ensures inputs are properly connected to character functionality.
