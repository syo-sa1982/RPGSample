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
    // tmx上のObjectsをobjectGroupに定義
    CCTMXObjectGroup *objectGroup = _tileMap->objectGroupNamed("Object");
    if (objectGroup == NULL) {
        CCLog("tile map has no objects object layer");
        return false;
    }
    
    // tmx上のSpawnPointの座標を取得
    CCDictionary *spawnPoint = objectGroup->objectNamed("SpawnPoint");
    
    int x = ((CCString)*spawnPoint->valueForKey("x")).intValue();
    int y = ((CCString)*spawnPoint->valueForKey("y")).intValue();
    
    _player = CCSprite::create("Player.png", CCRectMake(0, 0, 27, 40));
    
    
    // プレイヤーをタグ識別
    _player->setTag(1);
    
    // スプライトに座標セット
    _player->setPosition(ccp(x,y));
    
    // スプライトをレイヤに追加
    this->addChild(_player);
    
    // 画面の表示座標をセット
    this->setViewPlayerCenter();
    this->schedule(schedule_selector(HelloWorld::setViewPlayerCenter));
    
    // タッチを有効化
    this->setTouchEnabled(true);
    
    return true;
}

void HelloWorld::setViewPlayerCenter()
{
    //    CCLog("setViewPlayerCenter");
    CCPoint playerPos = ((CCSprite *)this->getChildByTag(1))->getPosition();
    
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    
    int x = MAX(playerPos.x, winSize.width/2);
    int y = MAX(playerPos.y, winSize.height/2);
    x = MIN(x, (_tileMap->getMapSize().width * this->_tileMap->getTileSize().width) - winSize.width / 2);
    y = MIN(y, (_tileMap->getMapSize().height * _tileMap->getTileSize().height) - winSize.height/2);
    CCPoint actualPosition = ccp(x, y);
    
    CCPoint centerOfView = ccp(winSize.width/2, winSize.height/2);
    CCPoint viewPoint = ccpSub(centerOfView, actualPosition);
    this->setPosition(viewPoint);
}

void HelloWorld::setPlayerPosition(CCPoint position)
{
    CCPoint tileCoord = this->tileCoordForPosition(position);
    int tileGid = _meta->tileGIDAt(tileCoord);
    if (tileGid) {
        CCDictionary *properties = _tileMap->propertiesForGID(tileGid);
        if (properties) {
            CCString *collision = new CCString();
            *collision = *properties->valueForKey("Collidable");
            if (collision && (collision->compare("true") == 0)) {
                return;
            }
            
            CCString *collectible = new CCString();
            *collectible = *properties->valueForKey("Collectable");
            if (collectible && (collectible->compare("true") == 0)) {
                _meta->removeTileAt(tileCoord);
//                _foreground->removeTileAt(tileCoord);
            }
        }
    }
    _player->setPosition(position);
}

CCPoint HelloWorld::tileCoordForPosition(CCPoint position)
{
    // タップ座標を取得
    CCPoint tilePoint = ccpSub(position, _tileMap->getPosition());
    
    // タイルの幅
    float fTileWidth = _tileMap->getTileSize().width;
    // タイルの高さ
    float fTileHeight = _tileMap->getTileSize().height;
    // タイルの行数
    float fTileRows = _tileMap->getMapSize().height;
    // タップ座標をタイル座標に変換
    tilePoint.x = (int)(tilePoint.x / fTileWidth);
    tilePoint.y = (int)((fTileRows * fTileHeight - tilePoint.y) / fTileHeight);
    
    return tilePoint;
}


void HelloWorld::registerWithTouchDispatcher()
{
    // シングルはこっち
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
}
bool HelloWorld::ccTouchBegan(CCTouch *touch, CCEvent *event)
{
    CCLog("Touch!");
    // タップした座標
    CCPoint touchLocation = touch->getLocationInView();
    touchLocation = CCDirector::sharedDirector()->convertToGL(touchLocation);
    touchLocation = this->convertToNodeSpace(touchLocation);
    
    // 目的地
    CCPoint destinationPos = this->getDestinationPos(touchLocation);
    
    // safety check on the bounds of the map
    if (destinationPos.x <= (_tileMap->getMapSize().width * _tileMap->getTileSize().width) &&
        destinationPos.y <= (_tileMap->getMapSize().height * _tileMap->getTileSize().height) &&
        destinationPos.y >= 0 &&
        destinationPos.x >= 0)
    {
        this->setPlayerPosition(destinationPos);
    }
    
    return true;
}


void HelloWorld::ccTouchEnded(CCTouch *touch, CCEvent *event)
{
    CCLog("TouchEnd!");
}

CCPoint HelloWorld::getDestinationPos(CCPoint touchLocation)
{
    CCPoint playerPos = ((CCSprite *)this->getChildByTag(1))->getPosition();
    CCPoint diff = ccpSub(touchLocation, playerPos);
    
    if ( abs(diff.x) > abs(diff.y) ) {
        if (diff.x > 0) {
            playerPos.x += _tileMap->getTileSize().width;
        } else {
            playerPos.x -= _tileMap->getTileSize().width;
        }
    } else {
        if (diff.y > 0) {
            playerPos.y += _tileMap->getTileSize().height;
        } else {
            playerPos.y -= _tileMap->getTileSize().height;
        }
    }
    
    return playerPos;
}
