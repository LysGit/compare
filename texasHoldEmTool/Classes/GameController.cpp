//
//  GameController.cpp
//  texasHoldEmTool
//
//  Created by 罗耀生 on 14-7-5.
//
//

#include "GameController.h"



using namespace cocos2d;




Scene* GameController::createScene()
{
    auto scene = Scene::create();
    auto layer = GameController::create();
    scene->addChild(layer);
    return scene;
}

bool GameController::init()
{
    if ( !Layer::init() )
    {
        return false;
    }

    Size screenSize = Director::getInstance()->getWinSize();
    Sprite *bg = Sprite::create( "bg.jpg" );
    bg->setPosition( Point(screenSize.width*0.5, screenSize.height*0.5) );
    this->addChild( bg );
    
    std::thread t1(&GameController::loadJsonFile,this);
    t1.join();
    
    return true;
}

void GameController::loadJsonFile(){
    
    Widget* widget = GUIReader::getInstance()->widgetFromJsonFile( "TexasHold'emPoker/TexasHold'emPoker.ExportJson" );
    this->addChild( widget );
    
    _panel_info  = static_cast<Layout*>(widget->getChildByName( "Panel_info" ));
    _panel_play  = static_cast<Layout*>(widget->getChildByName( "Panel_play" ));
    _panel_choose  = static_cast<Layout*>(widget->getChildByName( "Panel_choose" ));
    _panel_help  = static_cast<Layout*>(widget->getChildByName( "Panel_help" ));
    _panel_explain  = static_cast<Layout*>(widget->getChildByName( "Panel_explain" ));
    _panel_teach  = static_cast<Layout*>(widget->getChildByName( "Panel_teach" ));
    
    Button *button_help = static_cast<Button*>(widget->getChildByName("Button_help"));
    Button *button_explain = static_cast<Button*>(widget->getChildByName("Button_explain"));
    Button *button_share = static_cast<Button*>(widget->getChildByName("Button_share"));
    
    button_help->addTouchEventListener( CC_CALLBACK_2(GameController::selectEvent, this) );
    button_explain->addTouchEventListener( CC_CALLBACK_2(GameController::selectEvent, this) );
    button_share->addTouchEventListener( CC_CALLBACK_2(GameController::selectEvent, this) );
}

#pragma mark - Action set
void GameController::selectEvent(Ref *pSender, Widget::TouchEventType type){
    
    if ( TOUCH_EVENT_BEGAN == (int)type) {
        
        Button *btn = (Button *)pSender;
        
        switch ( btn->getTag() ) {
            case 10:
            {
                CCLOG("帮助");
            }
                break;
            case 11:
            {
                CCLOG("说明");
            }
                break;
            case 12:
            {
                CCLOG("分享");
            }
                break;
            default:
                break;
        }
        
    }
    
}