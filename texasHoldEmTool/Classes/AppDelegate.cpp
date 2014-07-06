#include "AppDelegate.h"
#include "GameController.h"
#include "GameData.h"

USING_NS_CC;

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
        glview = GLView::create("texasHoldEmTool");
        director->setOpenGLView(glview);
    }
    
    // turn on display FPS
    director->setDisplayStats( false );
    
    //------------------  shipei -------
    Size screenSize = glview->getFrameSize();
    Size designSize = Size(1136, 640);
    
    FileUtils* pFileUtils = CCFileUtils::getInstance();
    std::vector<std::string> searchPaths;
    
    if ( Platform::OS_IPAD == this->getTargetPlatform() ) {
        designSize = Size(1024, 768);
        searchPaths.push_back("iPad");
        
        memcpy( sysPlatform, "iPad", sizeof(char)*strlen("iPad") );
        
    }else if ( Platform::OS_IPHONE == this->getTargetPlatform() ) {
        float visibleSizeWidth = 960.f;
        float visibleSizeHeight =  640.f;

        if ( visibleSizeWidth/visibleSizeHeight ==  screenSize.width/screenSize.height ) {
            designSize = Size(960, 640);
            searchPaths.push_back("iPhone4");
            
            memcpy( sysPlatform, "iPhone4", sizeof(char)*strlen("iPhone4") );
            
        }else{
            designSize = Size(1136 , 640);
            searchPaths.push_back("iPhone5");
            
            memcpy( sysPlatform, "iPhone5", sizeof(char)*strlen("iPhone5") );
        }
    }else{
        designSize = Size(800 , 480);
        searchPaths.push_back("Android");
        
        memcpy( sysPlatform, "Android", sizeof(char)*strlen("Android") );
    }
    
    
    CCLOG("sysPlatform:%s", sysPlatform);
    
    searchPaths.push_back("TexasHold'emPoker");
    
//    SpriteFrameCache::getInstance()->addSpriteFramesWithFile( "TexasHold'emPoker0.plist" );
    
    pFileUtils->setSearchPaths(searchPaths);
    
    director->setContentScaleFactor(screenSize.height/screenSize.height);
    glview->setDesignResolutionSize(designSize.width, designSize.height, ResolutionPolicy::EXACT_FIT );
    
    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);
    

    
    std::thread t1(&AppDelegate::loadResources,this);
    t1.join();
    //  t1.detach();
    
    CCLOG("主线程");

    // create a scene. it's an autorelease object
    auto scene = GameController::createScene();
    
    // run
    director->runWithScene(scene);
    
    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}

void AppDelegate::loadResources(){
    CCLOG("加载资源文件...");
    
    //添加图片缓存
//    SpriteFrameCache::getInstance()->addSpriteFramesWithFile( "TexasHold'emPoker/TexasHold'emPoker0.plist" );
    
//    CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect( "Explo.mp3" );
//    CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect( "rush.mp3" );
}
