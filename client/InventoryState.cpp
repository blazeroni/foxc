#include "includes.h"
#include "InventoryState.h"
#include "MainGameState.h"
#include "Input.h"
#include "FrontEndGameState.h"
#include <sstream>

#define INVEN_THEME "resources/sounds/MainTheme.wav"
#define UNIT_BOX "resources/images/gui/loadout_unit.png"
#define UNIT_BOX_HL "resources/images/gui/loadout_unit_hl.png"
#define ITEM_BOX "resources/images/gui/loadout_item.png"
#define ITEM_BOX_HL "resources/images/gui/loadout_item_hl.png"
#define WEAPON_BOX "resources/images/gui/loadout_weapon.png"
#define WEAPON_BOX_HL "resources/images/gui/loadout_weapon_hl.png"
#define UNIT "resources/images/unit.png"
#define PISTOL_IMG "resources/images/gui/pistol_gui.png"
#define PISTOL_INV_IMG "resources/images/gui/pistol_gui_inv.png"
#define PISTOL_CLIP_IMG "resources/images/gui/pistol_clip_gui.png"
#define PISTOL_CLIP_INV_IMG "resources/images/gui/pistol_clip_gui_inv.png"
#define GRENADE_IMG "resources/images/gui/grenade_gui.png"
#define GRENADE_INV_IMG "resources/images/gui/grenade_gui_inv.png"

InventoryState::InventoryState(Game* app) : 
  GameState(app)
{
}

#define COST_UNIT 50
#define COST_PISTOL 30
#define COST_PISTOLCLIP 10
#define COST_GRENADE 30
#define pointsLeft (_pointsMax - _pointsSpent)

bool InventoryState::load_files()
{
    _music = Audio::instance().loadMusic(INVEN_THEME);
    Display::instance().loadFont("resources/fonts/FreeMono.ttf");
    _unitBox = Display::instance().loadImage(UNIT_BOX);
    _unitBoxHL = Display::instance().loadImage(UNIT_BOX_HL);
    _itemBox = Display::instance().loadImage(ITEM_BOX);
    _itemBoxHL = Display::instance().loadImage(ITEM_BOX_HL);
    _weaponBox = Display::instance().loadImage(WEAPON_BOX);
    _weaponBoxHL = Display::instance().loadImage(WEAPON_BOX_HL);
    _unitImage = Display::instance().loadImage(UNIT);
    _pistolImage = Display::instance().loadImage(PISTOL_IMG);
    _pistolInvImage = Display::instance().loadImage(PISTOL_INV_IMG);
    _pistolClipImage = Display::instance().loadImage(PISTOL_CLIP_IMG);
    _pistolClipInvImage = Display::instance().loadImage(PISTOL_CLIP_INV_IMG);
    _grenadeImage = Display::instance().loadImage(GRENADE_IMG);
    _grenadeInvImage = Display::instance().loadImage(GRENADE_INV_IMG);
    return true;
}



void InventoryState::init()
{
    load_files();
    _pointsMax = 200;
    _pointsSpent = 0;
    _playerName = "Name";
    _selectedUnit = 0;
    _selectedInv = 0;
    for ( int i = 0; i < 8; ++i )
    {
        _unit[i] = -1;
        for ( int j = 0; j < 7; ++j )
            _loadout[i][j] = (itemtype)0;
    }
    _unit[0] = 0;
    //Display& d = Display::instance();
    //Audio& a = Audio::instance();

    //a.playMusic(-1, _music);		
}

int cost( itemtype item )
{
    if ( item == (itemtype)0 )
        return 0;
    if ( item == PISTOL )
        return COST_PISTOL;
    if ( item == PISTOLCLIP )
        return COST_PISTOLCLIP;
    if ( item == GRENADE )
        return COST_GRENADE;
}

void InventoryState::processSDLEvent(SDL_Event& event)
{
    Display& d = Display::instance();
    if ( event.type == SDL_MOUSEBUTTONUP )
    {
        if ( event.button.button == SDL_BUTTON_LEFT )
        {
            Point click = Input::instance().getMousePosition();
            // units
            for ( int i = 0; i < 8; ++i )
            {
                if ( click.x > 30 + i*(_unitBox->w+15) && click.x < 30 + i*(_unitBox->w+15) + _unitBox->w &&
                click.y > 50 && click.y < 50 + _unitBox->h )
                {
                    if ( _unit[i] >= 0 )
                        _selectedUnit = i;
                    else if ( pointsLeft > COST_UNIT )
                    {
                        _unit[i] = COST_UNIT;
                        _selectedUnit = i;
                        _pointsSpent += COST_UNIT;
                    }
                }
            }
            // hands
            for ( int i = 0; i < 2; ++i )
            {
                if ( click.x > 50 + i*(_weaponBox->w+15) && click.x < 50 + i*(_weaponBox->w+15) + _weaponBox->w &&
                click.y > 200 && click.y < 200 + _weaponBox->h )
                    _selectedInv = i;
            }
            // inv
            for ( int i = 0; i < 5; ++i )
            {
                if ( click.x > 300 + i*(_itemBox->w+15) && click.x < 300 + i*(_itemBox->w+15) + _itemBox->w &&
                click.y > 200 && click.y < 200 + _itemBox->h )
                    _selectedInv = i+2;
            }
            // weapons
            if ( click.x > 50 + 0*(_weaponBox->w+15) && click.x < 50 + 0*(_weaponBox->w+15) + _weaponBox->w &&
            click.y > 350 && click.y < 350 + _weaponBox->h )
            {
                if ( pointsLeft >= cost(PISTOL) )
                {
                    _loadout[_selectedUnit][_selectedInv] = PISTOL;
                    _pointsSpent += cost( PISTOL );
                }
            }
            else if ( click.x > 50 + 0*(_weaponBox->w+_itemBox->w+15)+_weaponBox->w+5 && click.x < 50 + 0*(_weaponBox->w+_itemBox->w+15)+_weaponBox->w+5 + _itemBox->w &&
            click.y > 350 && click.y < 350 + _itemBox->h )
            {
                if ( pointsLeft >= cost(PISTOLCLIP) )
                {
                    _loadout[_selectedUnit][_selectedInv] = PISTOLCLIP;
                    _pointsSpent += cost( PISTOLCLIP );
                }
            }
        }
        // deletion
        if ( event.button.button == SDL_BUTTON_RIGHT )
        {
            Point click = Input::instance().getMousePosition();
            // units
            for ( int i = 1; i < 8; ++i )
            {
                if ( click.x > 30 + i*(_unitBox->w+15) && click.x < 30 + i*(_unitBox->w+15) + _unitBox->w &&
                click.y > 50 && click.y < 50 + _unitBox->h )
                {
                    for ( int j = 0; j < 7; ++j )
                    {
                        _pointsSpent -= cost(_loadout[i][j]);
                        _loadout[i][j] = (itemtype)0;
                    }
                    _selectedUnit = 0;
                    _pointsSpent -= _unit[i];
                    _unit[i] = -1;
                }
            }
            // hands
            for ( int i = 0; i < 2; ++i )
            {
                if ( click.x > 50 + i*(_weaponBox->w+15) && click.x < 50 + i*(_weaponBox->w+15) + _weaponBox->w &&
                click.y > 200 && click.y < 200 + _weaponBox->h )
                {
                    _pointsSpent -= cost( _loadout[_selectedUnit][i] );
                    _loadout[_selectedUnit][i] = (itemtype)0;
                }
            }
            // inv
            for ( int i = 0; i < 5; ++i )
            {
                if ( click.x > 300 + i*(_itemBox->w+15) && click.x < 300 + i*(_itemBox->w+15) + _itemBox->w &&
                click.y > 200 && click.y < 200 + _itemBox->h )
                {
                    _pointsSpent -= cost( _loadout[_selectedUnit][i+2] );
                    _loadout[_selectedUnit][i+2] = (itemtype)0;
                }
            }
        }
    }
}

void InventoryState::update(uint32 X)
{
    Display& d = Display::instance();
    Audio& a = Audio::instance();
    
    // top data
    d.draw(10, 10, "Player:__"+_playerName);
    stringstream ss;
    string text;
    ss << _pointsSpent;
    ss >> text;
    d.draw( 450, 10, "Points_Spent:__"+text);
    ss.clear(stringstream::goodbit);
    ss << _pointsMax - _pointsSpent;
    ss >> text;
    d.draw( 610, 10, "Points_Remaining:__"+text);

    // units
    for ( int i = 0; i < 8; ++i )
    {
        d.draw( 30 + (_unitBox->w+15)*(i), 50, _unitBox );
        if ( _unit[i] >= 0 )
            d.draw( 30 + (_unitBox->w+15)*(i)+_unitBox->w/2-_unitImage->w/2, 50+_unitBox->h/2-_unitImage->h/2, _unitImage );
    }
    if ( _selectedUnit >= 0 )
        d.draw( 30 + _selectedUnit*(_unitBox->w+15), 50, _unitBoxHL );
    // unit weapons
    d.draw( 50, 200, _weaponBox );
    d.draw( 50 + (_weaponBox->w+15), 200, _weaponBox );
    if ( _selectedInv == 0 || _selectedInv == 1 )
            d.draw( 50 + _selectedInv*(_weaponBox->w+15), 200, _weaponBoxHL );
    for ( int i = 0; i < 2; ++i )
    {
        if ( _loadout[_selectedUnit][i] > 0 )
        {
            if ( _loadout[_selectedUnit][i] == PISTOL )
                d.draw( 50 + i*(_weaponBox->w+15)+_weaponBox->w/2-_pistolImage->w/2, 200+_weaponBox->h/2-_pistolImage->h/2, _pistolImage );
            else if ( _loadout[_selectedUnit][i] == PISTOLCLIP )
                d.draw( 50 + i*(_weaponBox->w+15)+_weaponBox->w/2-_pistolClipImage->w/2, 200+_weaponBox->h/2-_pistolClipImage->h/2, _pistolClipImage );
            else if ( _loadout[_selectedUnit][i] == GRENADE )
                d.draw( 50 + i*(_weaponBox->w+15)+_weaponBox->w/2-_grenadeImage->w/2, 200+_weaponBox->h/2-_grenadeImage->h/2, _grenadeImage );
        }
    }
    // unit items
    for ( int i = 0; i < 5; ++i )
        d.draw( 300 + (_itemBox->w+15)*(i), 200, _itemBox );
    if ( _selectedInv >= 2 )
            d.draw( 300 + (_selectedInv-2)*(_itemBox->w+15), 200, _itemBoxHL );
    for ( int i = 0; i < 5; ++i )
    {
        if ( _loadout[_selectedUnit][i+2] > 0 )
        {
            if ( _loadout[_selectedUnit][i+2] == PISTOL )
                d.draw( 300 + i*(_itemBox->w+15)+_itemBox->w/2-_pistolInvImage->w/2, 200+_itemBox->h/2-_pistolInvImage->h/2, _pistolInvImage );
            else if ( _loadout[_selectedUnit][i+2] == PISTOLCLIP )
                d.draw( 300 + i*(_itemBox->w+15)+_itemBox->w/2-_pistolClipInvImage->w/2, 200+_itemBox->h/2-_pistolClipInvImage->h/2, _pistolClipInvImage );
            else if ( _loadout[_selectedUnit][i+2] == GRENADE )
                d.draw( 300 + i*(_itemBox->w+15)+_itemBox->w/2-_grenadeInvImage->w/2, 200+_itemBox->h/2-_grenadeInvImage->h/2, _grenadeImage );
        }
    }
    // weapons
    for ( int i = 0; i < 4; ++i )
    {
        d.draw( 50 + (_weaponBox->w+_itemBox->w+15)*(i), 350, _weaponBox );
        d.draw( 50 + (_weaponBox->w+_itemBox->w+15)*(i)+_weaponBox->w+5, 350, _itemBox );
    }
        // pistol
    d.draw( 50 + (_weaponBox->w+_itemBox->w+15)*(0)+_weaponBox->w/2-_pistolImage->w/2, 350+_weaponBox->h/2-_pistolImage->h/2, _pistolImage );
    d.draw( 50 + (_weaponBox->w+_itemBox->w+15)*(0)+_weaponBox->w+5+_itemBox->w/2-_pistolClipInvImage->w/2, 350+_itemBox->h/2-_pistolClipInvImage->h/2, _pistolClipInvImage );
    // items
    for ( int i = 0; i < 6; ++i )
        d.draw( 50 + (_weaponBox->w+15)*(i), 475, _weaponBox );

    SDL_Delay( 200 );
}

void InventoryState::deinit()
{
}

