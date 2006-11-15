#include "includes.h"
#include "InventoryState.h"
#include "MainGameState.h"
#include "Input.h"
#include "FrontEndGameState.h"
#include "xcore/UnitCreateEvent.h"
#include "xcore/GameJoinEvent.h"
#include "xcore/GameListEvent.h"
#include "xcore/PlayerJoinEvent.h"
#include "ClientNetwork.h"
#include "ConfigOptions.h"

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
#define RPG_IMG "resources/images/gui/rpg_gui.png"
#define RPG_IMG_INV "resources/images/gui/rpg_gui_inv.png"
#define ROCKET_IMG "resources/images/gui/rocket_gui.png"
#define ROCKET_IMG_INV "resources/images/gui/rocket_gui_inv.png"
#define RIFLE_IMG "resources/images/gui/rifle_gui.png"
#define RIFLE_IMG_INV "resources/images/gui/rifle_gui_inv.png"
#define RIFLECLIP_IMG "resources/images/gui/rifleclip_gui.png"
#define RIFLECLIP_IMG_INV "resources/images/gui/rifleclip_gui_inv.png"
#define GRENADE_IMG "resources/images/gui/grenade_gui.png"
#define GRENADE_INV_IMG "resources/images/gui/grenade_gui_inv.png"
#define MEDKIT_IMG "resources/images/gui/medkit_gui.png"
#define MEDKIT_INV_IMG "resources/images/gui/medkit_gui_inv.png"
#define STIM_IMG "resources/images/gui/stim_gui.png"
#define STIM_INV_IMG "resources/images/gui/stim_gui_inv.png"
#define READY_IMG "resources/images/gui/ready.png"

InventoryState::InventoryState(Game* app, spPlayer player, uint32 maxPoints) :
  GameState(app),
  _localPlayer(player),
  _pointsMax(maxPoints)
{
    //_ready = false;
}

#define COST_UNIT 50
#define COST_PISTOL 30
#define COST_PISTOLCLIP 10
#define COST_GRENADE 30
#define COST_MEDKIT 30
#define COST_RPGL 70
#define COST_ROCKET 30
#define COST_RIFLE 50
#define COST_RIFLECLIP 15
#define COST_STIM 30
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
    _rpgImage = Display::instance().loadImage(RPG_IMG);
    _rpgInvImage = Display::instance().loadImage(RPG_IMG_INV);
    _rocketImage = Display::instance().loadImage(ROCKET_IMG);
    _rocketInvImage = Display::instance().loadImage(ROCKET_IMG_INV);
    _rifleImage = Display::instance().loadImage(RIFLE_IMG);
    _rifleInvImage = Display::instance().loadImage(RIFLE_IMG_INV);
    _rifleClipImage = Display::instance().loadImage(RIFLECLIP_IMG);
    _rifleClipInvImage = Display::instance().loadImage(RIFLECLIP_IMG_INV);
    _grenadeImage = Display::instance().loadImage(GRENADE_IMG);
    _grenadeInvImage = Display::instance().loadImage(GRENADE_INV_IMG);
    _medkitImage = Display::instance().loadImage(MEDKIT_IMG);
    _medkitInvImage = Display::instance().loadImage(MEDKIT_INV_IMG);
    _stimImage = Display::instance().loadImage(STIM_IMG);
    _stimInvImage = Display::instance().loadImage(STIM_INV_IMG);
    _readyImage = Display::instance().loadImage(READY_IMG);
    return true;
}

string InventoryState::getName()
{
   return "Inventory";
}

void InventoryState::init()
{
    //ConfigOptions& o = ConfigOptions::instance();
    //ClientNetwork &cn = ClientNetwork::instance();
    //cn.connectToServer(o.get<string>(HOSTNAME).c_str(), o.get<int>(PORT));
    //bool host = true;
    //cin >> host;
    //ClientNetwork::instance().send(GameListEvent());
    //cn.send(PlayerJoinEvent(ConfigOptions::instance().get<string>(PLAYER_NAME)));
    //cn.send(GameJoinEvent(ConfigOptions::instance().get<string>(GAME_NAME)));
    load_files();
    _pointsSpent = 0;
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
    //if ( item == (itemtype)0 )
    //    return 0;
    if ( item == PISTOL )
        return COST_PISTOL;
    if ( item == PISTOLCLIP )
        return COST_PISTOLCLIP;
    if ( item == GRENADE )
        return COST_GRENADE;
    if ( item == MEDKIT )
        return COST_MEDKIT;
    if ( item == STIM )
        return COST_STIM;
    if ( item == RPGL )
        return COST_RPGL;
    if ( item == ROCKET )
        return COST_ROCKET;
    if ( item == RIFLE )
        return COST_RIFLE;
    if ( item == RIFLECLIP )
        return COST_RIFLECLIP;

    return 0;
}

void InventoryState::processSDLEvent(SDL_Event& event)
{
    Display& d = Display::instance();
    if ( event.type == SDL_MOUSEBUTTONUP )
    {
        if ( event.button.button == SDL_BUTTON_LEFT )
        {
            Point click = Input::instance().getMousePosition();
            if ( click.x > 650 && click.x < 650+_readyImage->w && click.y > 200 && click.y < 200+_readyImage->h )
            {
               commit();
               return;
            }
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
                // pistol
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
                // rifle
            if ( click.x > 50 + 1*(_weaponBox->w+15) && click.x < 50 + 1*(_weaponBox->w+15) + _weaponBox->w &&
            click.y > 350 && click.y < 350 + _weaponBox->h )
            {
                if ( pointsLeft >= cost(RIFLE) )
                {
                    _loadout[_selectedUnit][_selectedInv] = RIFLE;
                    _pointsSpent += cost( RIFLE );
                }
            }
            else if ( click.x > 50 + 1*(_weaponBox->w+_itemBox->w+15)+_weaponBox->w+5 && click.x < 50 + 1*(_weaponBox->w+_itemBox->w+15)+_weaponBox->w+5 + _itemBox->w &&
            click.y > 350 && click.y < 350 + _itemBox->h )
            {
                if ( pointsLeft >= cost(RIFLECLIP) )
                {
                    _loadout[_selectedUnit][_selectedInv] = RIFLECLIP;
                    _pointsSpent += cost( RIFLECLIP );
                }
            }
                // rpg
            else if ( click.x > 50 + 2*(_weaponBox->w+_itemBox->w+15) && click.x < 50 + 2*(_weaponBox->w+_itemBox->w+15) + _itemBox->w &&
            click.y > 350 && click.y < 350 + _itemBox->h )
            {
                if ( pointsLeft >= cost(RPGL) )
                {
                    _loadout[_selectedUnit][_selectedInv] = RPGL;
                    _pointsSpent += cost( RPGL );
                }
            }
            else if ( click.x > 50 + 2*(_weaponBox->w+_itemBox->w+15)+_weaponBox->w+5 && click.x < 50 + 2*(_weaponBox->w+_itemBox->w+15)+_weaponBox->w+5 + _itemBox->w &&
            click.y > 350 && click.y < 350 + _itemBox->h )
            {
                if ( pointsLeft >= cost(ROCKET) )
                {
                    _loadout[_selectedUnit][_selectedInv] = ROCKET;
                    _pointsSpent += cost( ROCKET );
                }
            }
            // items
            else if ( click.x > 50 + 0*(_weaponBox->w+15) && click.x < 50 + 0*(_weaponBox->w+15)+_weaponBox->w &&
            click.y > 475 && click.y < 475 + _weaponBox->h )
            {
                if ( pointsLeft >= cost(GRENADE) )
                {
                    _loadout[_selectedUnit][_selectedInv] = GRENADE;
                    _pointsSpent += cost( GRENADE );
                }
            }
            else if ( click.x > 50 + 1*(_weaponBox->w+15) && click.x < 50 + 1*(_weaponBox->w+15)+_weaponBox->w &&
            click.y > 475 && click.y < 475 + _weaponBox->h )
            {
                if ( pointsLeft >= cost(MEDKIT) )
                {
                    _loadout[_selectedUnit][_selectedInv] = MEDKIT;
                    _pointsSpent += cost( MEDKIT );
                }
            }
            else if ( click.x > 50 + 2*(_weaponBox->w+15) && click.x < 50 + 2*(_weaponBox->w+15)+_weaponBox->w &&
            click.y > 475 && click.y < 475 + _weaponBox->h )
            {
                if ( pointsLeft >= cost(STIM) )
                {
                    _loadout[_selectedUnit][_selectedInv] = STIM;
                    _pointsSpent += cost( STIM );
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
 //   if ( !_ready )
 //   {
	//Display::instance().draw(275, 300, "Waiting for players...");
	//return;
 //   }
    Display& d = Display::instance();
    Audio& a = Audio::instance();
    
    // top data
    d.draw(10, 10, "Player: "+ _localPlayer->getName());
    stringstream ss;
    string text;
    ss << _pointsSpent;
    ss >> text;
    d.draw( 450, 10, "Points Spent: " +text);
    ss.clear(stringstream::goodbit);
    ss << _pointsMax - _pointsSpent;
    ss >> text;
    d.draw( 610, 10, "Points Remaining: "+text);

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
            else if ( _loadout[_selectedUnit][i] == MEDKIT )
                d.draw( 50 + i*(_weaponBox->w+15)+_weaponBox->w/2-_medkitImage->w/2, 200+_weaponBox->h/2-_medkitImage->h/2, _medkitImage );
            else if ( _loadout[_selectedUnit][i] == RPGL )
                d.draw( 50 + i*(_weaponBox->w+15)+_weaponBox->w/2-_rpgImage->w/2, 200+_weaponBox->h/2-_rpgImage->h/2, _rpgImage );
            else if ( _loadout[_selectedUnit][i] == ROCKET )
                d.draw( 50 + i*(_weaponBox->w+15)+_weaponBox->w/2-_rocketImage->w/2, 200+_weaponBox->h/2-_rocketImage->h/2, _rocketImage );
            else if ( _loadout[_selectedUnit][i] == STIM )
                d.draw( 50 + i*(_weaponBox->w+15)+_weaponBox->w/2-_stimImage->w/2, 200+_weaponBox->h/2-_stimImage->h/2, _stimImage );
            else if ( _loadout[_selectedUnit][i] == RIFLE )
                d.draw( 50 + i*(_weaponBox->w+15)+_weaponBox->w/2-_rifleImage->w/2, 200+_weaponBox->h/2-_rifleImage->h/2, _rifleImage );
            else if ( _loadout[_selectedUnit][i] == RIFLECLIP )
                d.draw( 50 + i*(_weaponBox->w+15)+_weaponBox->w/2-_rifleClipImage->w/2, 200+_weaponBox->h/2-_rifleClipImage->h/2, _rifleClipImage );
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
                d.draw( 300 + i*(_itemBox->w+15)+_itemBox->w/2-_grenadeInvImage->w/2, 200+_itemBox->h/2-_grenadeInvImage->h/2, _grenadeInvImage );
            else if ( _loadout[_selectedUnit][i+2] == MEDKIT )
                d.draw( 300 + i*(_itemBox->w+15)+_itemBox->w/2-_medkitInvImage->w/2, 200+_itemBox->h/2-_medkitInvImage->h/2, _medkitInvImage );
            else if ( _loadout[_selectedUnit][i+2] == RPGL )
                d.draw( 300 + i*(_itemBox->w+15)+_itemBox->w/2-_rpgInvImage->w/2, 200+_itemBox->h/2-_rpgInvImage->h/2, _rpgInvImage );
            else if ( _loadout[_selectedUnit][i+2] == ROCKET )
                d.draw( 300 + i*(_itemBox->w+15)+_itemBox->w/2-_rocketInvImage->w/2, 200+_itemBox->h/2-_rocketInvImage->h/2, _rocketInvImage );
            else if ( _loadout[_selectedUnit][i+2] == STIM )
                d.draw( 300 + i*(_itemBox->w+15)+_itemBox->w/2-_stimInvImage->w/2, 200+_itemBox->h/2-_stimInvImage->h/2, _stimInvImage );
            else if ( _loadout[_selectedUnit][i+2] == RIFLE )
                d.draw( 300 + i*(_itemBox->w+15)+_itemBox->w/2-_rifleImage->w/2, 200+_itemBox->h/2-_rifleImage->h/2, _rifleImage );
            else if ( _loadout[_selectedUnit][i+2] == RIFLECLIP )
                d.draw( 300 + i*(_itemBox->w+15)+_itemBox->w/2-_rifleClipImage->w/2, 200+_itemBox->h/2-_rifleClipImage->h/2, _rifleClipImage );
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
        // rifle
    d.draw( 50 + (_weaponBox->w+_itemBox->w+15)*(1)+_weaponBox->w/2-_rifleImage->w/2, 350+_weaponBox->h/2-_rifleImage->h/2, _rifleImage );
    d.draw( 50 + (_weaponBox->w+_itemBox->w+15)*(1)+_weaponBox->w+5+_itemBox->w/2-_rifleClipInvImage->w/2, 350+_itemBox->h/2-_rifleClipInvImage->h/2, _rifleClipInvImage );
        // rpg
    d.draw( 50 + (_weaponBox->w+_itemBox->w+15)*(2)+_weaponBox->w/2-_rpgImage->w/2, 350+_weaponBox->h/2-_rpgImage->h/2, _rpgImage );
    d.draw( 50 + (_weaponBox->w+_itemBox->w+15)*(2)+_weaponBox->w+5+_itemBox->w/2-_rocketInvImage->w/2, 350+_itemBox->h/2-_rocketInvImage->h/2, _rocketInvImage );
    // items
    for ( int i = 0; i < 6; ++i )
        d.draw( 50 + (_weaponBox->w+15)*(i), 475, _weaponBox );
	// grenade
    d.draw( 50 + (_weaponBox->w)*(0)+_weaponBox->w/2-_grenadeImage->w/2, 475+_weaponBox->h/2-_grenadeImage->h/2, _grenadeImage );
        // medkit
    d.draw( 50 + (_weaponBox->w+15)*(1)+_weaponBox->w/2-_medkitImage->w/2, 475+_weaponBox->h/2-_medkitImage->h/2, _medkitImage );
        // stimpack
    d.draw( 50 + (_weaponBox->w+15)*(2)+_weaponBox->w/2-_stimImage->w/2, 475+_weaponBox->h/2-_stimImage->h/2, _stimImage );
    // ready button
    d.draw( 650, 200, _readyImage );


    SDL_Delay( 20 );
}

void InventoryState::deinit()
{
}

void InventoryState::setLocalPlayer(spPlayer player)
{
   _localPlayer = player;
}

void InventoryState::commit()
{
   ClientNetwork &cn = ClientNetwork::instance();
   for ( int i = 0; i < 8; ++i )
   {
      if ( _unit[i] >= 0 )
      {
         //cn.send( UnitCreateEvent(_playerID, 0, 0,
         cn.send( UnitCreateEvent(_localPlayer->getID(), 0, 0,
         _loadout[i][1],
         _loadout[i][0],
         _loadout[i][6],
         _loadout[i][5],
         _loadout[i][4],
         _loadout[i][3],
         _loadout[i][2] ) ); 
      }
   }
   cn.send( StartGameEvent() );
   GameState* gs = new MainGameState(_game, _localPlayer);
   gs->init();
   _game->changeState(gs);				
}
