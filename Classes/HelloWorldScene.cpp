#include "HelloWorldScene.h"

USING_NS_CC;

CCScene* HelloWorld::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    HelloWorld *layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    
    // タイルマップ呼び出し
    _tileMap = new CCTMXTiledMap();
    _tileMap->initWithTMXFile("th_cobit_rouka_1f.tmx");
    
    //
    _wallFloor = _tileMap->layerNamed("WallFloor");
    // メタレイヤー
    _meta = _tileMap->layerNamed("Meta");
    // プレイヤーの目からは見えなくする
    _meta->setVisible(false);

    // タイルマップ表示
    this->addChild(_tileMap);
    
    
    // タッチを有効化
    this->setTouchEnabled(true);
    
    return true;
}

