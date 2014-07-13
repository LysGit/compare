//
//  GameController.cpp
//  texasHoldEmTool
//
//  Created by 罗耀生 on 14-7-5.
//
//

#include "GameController.h"
#include "GameData.h"
#include "pokerstove/PokerEvaluation.h"

#include "./C2DXShareSDK/C2DXShareSDK.h"

using namespace cn::sharesdk;
using namespace cocos2d;

void shareResultHandler(C2DXResponseState state, C2DXPlatType platType, __Dictionary *shareInfo, __Dictionary *error)
{
    log("分享回调 State：%d", state);
    //    shareInfo->~CCObject()
    switch (state) {
        case C2DXResponseStateSuccess:
            log("分享成功");
            break;
        case C2DXResponseStateFail:
            //            CCLog("error_msg: %s, error_code: %s", dynamic_cast<CCString*>(error->objectForKey("error_msg"))->getCString(), dynamic_cast<CCString*>(error->objectForKey("error_code"))->getCString(), NULL);
            log("分享失败");
            break;
        default:
            log("分享失败 : %d", state);
            break;
    }
}
void authResultHandler(C2DXResponseState state, C2DXPlatType platType, __Dictionary *error)
{
    switch (state) {
        case C2DXResponseStateSuccess:
            log("授权成功");
            break;
        case C2DXResponseStateFail:
            log("授权失败");
            break;
        default:
            break;
    }
}

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
    
    this->DATA_init();
    
    this->loadJsonFile();
    
    std::thread t1(&GameController::startLoad,this);
    t1.join();
    
    this->endLoad();
    
    return true;
}

void GameController::loadJsonFile(){
    
    std::string str = "TexasHold'emPoker_";
    str += g_sysPlatform;
    str += ".ExportJson";
    Layout* widget = (Layout*)GUIReader::getInstance()->widgetFromJsonFile( str.c_str() );
    this->addChild( widget );
    
    _panel_info  = static_cast<Layout*>(widget->getChildByName( "Panel_info" ));
    _panel_play  = static_cast<Layout*>(widget->getChildByName( "Panel_play" ));
    _panel_choose  = static_cast<Layout*>(widget->getChildByName( "Panel_choose" ));
    _panel_help  = static_cast<Layout*>(widget->getChildByName( "Panel_help" ));
    _panel_explain  = static_cast<Layout*>(widget->getChildByName( "Panel_explain" ));
//    _panel_menu  = static_cast<Layout*>(widget->getChildByName( "Panel_menu" ));
    _panel_teach  = static_cast<Layout*>(widget->getChildByName( "Panel_teach" ));
    
    
    Button *button_help = static_cast<Button*>(widget->getChildByName( "Button_help" ));
    Button *button_explain = static_cast<Button*>(widget->getChildByName( "Button_explain" ));
    Button *button_share = static_cast<Button*>(widget->getChildByName( "Button_share" ));
    Button *button_refresh = static_cast<Button*>(widget->getChildByName( "Button_refresh" ));
    
    button_help->addTouchEventListener( CC_CALLBACK_2(GameController::selectEvent_menu, this) );
    button_explain->addTouchEventListener( CC_CALLBACK_2(GameController::selectEvent_menu, this) );
    button_share->addTouchEventListener( CC_CALLBACK_2(GameController::selectEvent_menu, this) );
    button_refresh->addTouchEventListener( CC_CALLBACK_2(GameController::selectEvent_menu, this) );
    
    for ( auto& e : _panel_play->getChildren() ) {
        Button *btn = (Button *)e;
        btn->addTouchEventListener( CC_CALLBACK_2(GameController::selectEvent_play, this) );
    }
    
    for ( auto& e : _panel_choose->getChildren() ) {
        Button *btn = (Button *)e;
        btn->addTouchEventListener( CC_CALLBACK_2(GameController::selectEvent_choose, this) );
    }
    
    for (int idx = 0; idx < 5; idx++) {
        std::string s = StringUtils::format("poker_%d", idx+100) ;
        _choose_button[ idx ] = static_cast<Button*>(_panel_choose->getChildByName( s ));
        _choose_button[ idx ]->setPosition( Point(0.5, 0.5) );
        _choose_button[ idx ]->setTag( 999 );
    }
}

void GameController::startLoad(){
    CCLOG("加载等待动画");
}
void GameController::endLoad(){
    CCLOG("关闭等待动画");
}

#pragma mark - Data init
void GameController::DATA_init(){
    _iSelectIdx = 0;
    _iUserIdx = 0;
    for (int i = 0; i < 11; i++) {
        for (int j = 0; j < 5; j++) {
            _ucCard[ i ][ j ] = 10==i?70:0;
        }
    }
}

void GameController::DATA_calculate(){

    _bCalculate = false;
    
    unsigned char curCenterCardData[5] = { 0 };
    unsigned char handCardData[GAME_PLAYER][2] ;

    for (int idx = 0; idx < GAME_PLAYER; idx++) {
        
        handCardData[ idx ][0] = _ucCard[ idx ][0];
        handCardData[ idx ][1] = _ucCard[ idx ][1];
    }

    //公共牌
    for (int i = 0; i < 5; i++) {
        if ( _ucCard[10][i] == 70 ) {
            return;
        }
        curCenterCardData[ i ] = _ucCard[10][i];
    }

    PokerEvaluation::maxIdx_Evaluation( handCardData, curCenterCardData, _maxIdx);

    _bCalculate = true;
}

bool GameController::DATA_check(int tag){
    
    if ( tag == 0 || tag == 70) {
        return false;
    }
    
    for (int i = 0; i < 11; i++) {
        for (int j = 0; j < 5; j++) {
            
            if ( _ucCard[ i ][ j ] == tag ) {
                return true;
            }
        }
    }
    
    
    return false;
}
#pragma mark - UI update
void GameController::UI_updateInfo(){
    
    for (int idx = 0; idx < 11; idx++) {
        this->UI_updateInfo( idx );
    }
}
void GameController::UI_updateInfo(int iUser){
    
    int iMax = 10==iUser?5:2;
    
    for ( int idx = 0; idx < iMax; idx++) {
        std::string s = StringUtils::format("poker_%d", iUser*10+idx ) ;
        ImageView *ivPoker  = static_cast<ImageView*>(_panel_info->getChildByName( s ));
        std::string s1 = StringUtils::format("poker%d.png", _ucCard[ iUser ][ idx] ) ;
        ivPoker->loadTexture( s1 , TextureResType::PLIST);
    }
    if ( iUser < 10) {
        std::string s = StringUtils::format("win_%d", iUser) ;
        ImageView *ivWiner  = static_cast<ImageView*>(_panel_info->getChildByName( s ));
        
        int itmp = 0;
        for (int i = 0; i < 4; i++) {
            if( _maxIdx[ i ] > 0 && iUser == _maxIdx[ i ] ){
                itmp = 1;
                break;
            }
        }
        
        ivWiner->setVisible( itmp==1?true:false );
    }
}

void GameController::UI_updateWin(){
    
    for (int iUser = 0; iUser < 10; iUser++) {
        
        std::string s = StringUtils::format("win_%d", iUser) ;
        ImageView *ivWiner  = static_cast<ImageView*>(_panel_info->getChildByName( s ));
        
        int itmp = 0;
        for (int i = 0; i < 4; i++) {
            if( _maxIdx[ i ] >= 0 && iUser == _maxIdx[ i ] ){
                itmp = 1;
                break;
            }
        }
        
        ivWiner->setVisible( itmp==1?true:false );
    }
    
}
#pragma mark - Action set
void GameController::selectEvent_menu(Ref *pSender, Widget::TouchEventType type){
    
    if ( TOUCH_EVENT_BEGAN == (int)type) {
        
        Button *btn = (Button *)pSender;
        
        switch ( btn->getTag() ) {
            case 10:
            {
                CCLOG("帮助");
                //添加进度条 -- 需要添加一个层，有灰度
            }
                break;
            case 11:
            {
                CCLOG("说明");
                ArmatureDataManager::getInstance()->addArmatureFileInfo("LoadingAnimation.ExportJson");
                Armature* armature = Armature::create("LoadingAnimation");
                armature->setTag( 10086 );
                
                Size size = Director::getInstance()->getWinSize();
                
                armature->setPosition(Point( size.width/2 , size.height/2));
                this->addChild(armature);
                
                armature->getAnimation()->play("Animation1");
                
                std::thread t1(&GameController::DATA_calculate,this);
                t1.join();
                
                armature->removeFromParentAndCleanup( true );
                
                if ( _bCalculate ) {
                    CCLOG("得到答案！");
                    this->UI_updateWin();
                    
                }else{
                    CCLOG("提示错误！");
                }
            }
                break;
            case 12:
            {
                CCLOG("分享");
                __Dictionary *content = Dictionary::create();
                content -> setObject(CCString::create("准了!#德州扑克保险计算App#增加了我的胜率!连胜率100%!下载分享转发,随机赢取ipad! http://www.mbp.com/home/article/lists/category/58.html"), "content");
                content -> setObject(CCString::create("http://www.mbp.com/pics/oddsshare.jpg"), "image");
                content -> setObject(CCString::create("亿濠德州保险赔率"), "title");
                content -> setObject(CCString::create("亿濠德州保险赔率"), "description");
                content -> setObject(CCString::create("http://www.mbp.com/home/article/lists/category/58.html"), "url");
                
                content -> setObject(CCString::createWithFormat("%d", C2DXContentTypeNews), "type");
                C2DXShareSDK::showShareMenu(NULL, content, Point(100, 100), C2DXMenuArrowDirectionLeft, shareResultHandler);
            }
                break;
            default:
            {
                CCLOG("刷新");
                Director::getInstance()->replaceScene( GameController::createScene() );
            }
                break;
        }
        
    }
}

void GameController::selectEvent_choose(Ref *pSender, Widget::TouchEventType type){
    
    if ( TOUCH_EVENT_BEGAN == (int)type) {
        
        Button *btn = (Button *)pSender;
        std::string s = btn->getName();
        int tag = atoi( s.substr( s.find('_')+1, s.size() ).c_str() );
        switch ( tag ) {
            case 0:
            {
                _panel_choose->setVisible( false );
                
                //更新info层
                this->UI_updateInfo();
                
            }
                break;
                
            default:
            {
                if ( tag >= 100) {
                    _iSelectIdx = tag - 100;
                    
                    if ( _choose_button[ _iSelectIdx ]->getTag() != 999) {

                        std::string s2 = StringUtils::format("poker_%d", _choose_button[ _iSelectIdx ]->getTag()) ;
                        Button *btn1 = (Button *)_panel_choose->getChildByName( s2 );
                        btn1->setBright( true );
                        
                        _choose_button[ _iSelectIdx ]->setVisible( true );
                        _choose_button[ _iSelectIdx ]->loadTextures( "poker70.png", "","",  TextureResType::PLIST);
                        _choose_button[ _iSelectIdx ]->setTag( 999 );

                        _ucCard[ _iUserIdx ][ _iSelectIdx ] = 10==_iUserIdx?70:0;
                    }
                }else{
                    
                    if ( DATA_check( tag ) ) {
                        return;
                    }
                    
                    if ( _iSelectIdx >= _iMax) {
//                        _iSelectIdx--;
                        return;
                    }
                    
                    if ( _ucCard[ _iUserIdx ][ _iSelectIdx ] != 70 && _ucCard[ _iUserIdx ][ _iSelectIdx ] != 0 ) {
                        
                        std::string s2 = StringUtils::format("poker_%d", _choose_button[ _iSelectIdx ]->getTag()) ;
                        Button *btn1 = (Button *)_panel_choose->getChildByName( s2 );
                        btn1->setBright( true );
                        
                        _choose_button[ _iSelectIdx ]->setVisible( true );
                        _choose_button[ _iSelectIdx ]->loadTextures( "poker70.png", "","",  TextureResType::PLIST);
                        _choose_button[ _iSelectIdx ]->setTag( 999 );
                        
                        _ucCard[ _iUserIdx ][ _iSelectIdx ] = 10==_iUserIdx?70:0;
                        
                    }
                    
                    btn->setBright( false );
                    
                    std::string s1 = StringUtils::format("poker%d.png", tag) ;
                    _choose_button[ _iSelectIdx ]->loadTextures( s1, "poker70.png", "", TextureResType::PLIST);
                    _choose_button[ _iSelectIdx ]->setTag( tag );
                    
                    _ucCard[ _iUserIdx ][ _iSelectIdx ] = tag;
                    
                    _iSelectIdx++;
                }
            }
                break;
        }
    }
    //截取字符
}

void GameController::selectEvent_play(Ref *pSender, Widget::TouchEventType type){
    
    if ( TOUCH_EVENT_BEGAN == (int)type) {
        
        _iSelectIdx = 0;
        _panel_choose->setVisible( true );

        Button *btn = (Button *)pSender;
        std::string s = btn->getName();
        _iUserIdx = atoi( s.substr( s.find('_')+1, s.size() ).c_str() );
        _iMax = 10==_iUserIdx?5:2;

        //动画
        for (int idx = 0; idx < 5; idx++) {
            
            _choose_button[ idx ]->setTag( 999 );
            _choose_button[ idx ]->loadTextures( "poker70.png", "","",  TextureResType::PLIST);
            
            if ( idx < _iMax ) {
                _choose_button[ idx ]->setVisible( true );
                
                std::string s1 = StringUtils::format( "poker%d.png", _ucCard[ _iUserIdx ][ idx ]==0?70:_ucCard[ _iUserIdx ][ idx ] ) ;
                _choose_button[ idx ]->loadTextures( s1, "poker70.png", "", TextureResType::PLIST);
                _choose_button[ idx ]->setTag( _ucCard[ _iUserIdx ][ idx ] );
            }else{
                _choose_button[ idx ]->setVisible( false );
            }
        }
        
    }
}
