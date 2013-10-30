#include "HelloWorldScene.h"

USING_NS_CC;


enum {
    kTagTileMap = 1,
    kTagPlayer  = 2,
};

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
    _tileMap = CCTMXTiledMap::create("th_cobit_rouka_1f.tmx");
    this->addChild(_tileMap, 0, kTagTileMap);
    
    CCSize CC_UNUSED tileSize = _tileMap->getContentSize();
    CCLOG("ContentSize: %f, %f", tileSize.width,tileSize.height);
    
    
    _player = CCSprite::create("Player.png");
    
    
    CCTMXObjectGroup *objectGroup = _tileMap->objectGroupNamed("Object");
    if (objectGroup == NULL) {
        CCLog("tile map has no objects object layer");
        return false;
    }
    
    // tmx上のSpawnPointの座標を取得
    CCDictionary *spawnPoint = objectGroup->objectNamed("SpawnPoint");
    
    int x = ((CCString)*spawnPoint->valueForKey("x")).intValue();
    int y = ((CCString)*spawnPoint->valueForKey("y")).intValue();
    
    std::cout << x << std::endl;
    std::cout << y << std::endl;
    // プレイヤーをタグ識別
    _player->setTag(kTagPlayer);
    
    // スプライトに座標セット
    _player->setPosition(ccp(x,y));
    
    _tileMap->addChild(_player, 1);
    
    
    CCPoint playerPos = _player->getPosition();
    
    CCLog("get playerPos!!!");
    std::cout << playerPos.x << std::endl;
    std::cout << playerPos.y << std::endl;
    
//    _tileMap->addChild(_player, _tileMap->getChildren()->count());
//    _player->retain();
//    _player->setAnchorPoint(ccp(-40,-10));
    
    
//    CCPoint p = _player->getPosition();
//    p = CC_POINT_POINTS_TO_PIXELS(p);
//    
//    
//    std::cout << "init" << std::endl;
//    std::cout << p.x << std::endl;
//    std::cout << p.y << std::endl;
    
//    CCPoint p = _player->getPosition();
//    
//    std::cout << p.x << std::endl;
//    std::cout << p.y << std::endl;
//    p = CC_POINT_POINTS_TO_PIXELS(p);
//    int newZ = 6 - ( (p.y-10) / 81);
//    newZ = fmax(newZ,0);
//    
//    _tileMap->reorderChild(_player, newZ);
//
//    _wallThrough = _tileMap->layerNamed("WallThrough");
//    _wallFloor = _tileMap->layerNamed("WallFloor");
    // メタレイヤー
    _meta = _tileMap->layerNamed("Meta");
    // プレイヤーの目からは見えなくする
    _meta->setVisible(false);

    // タイルマップ表示
//    this->addChild(_wallThrough, 3);
//    this->addChild(_wallFloor, 1);
    //    this->addChild(_tileMap);
//    this->addChild(_tileMapTop, 3);

    
    // tmx上のObjectsをobjectGroupに定義
    
//    CCActionInterval* move = CCMoveBy::create(10, ccp(-40,-200));
//    CCActionInterval* back = move->reverse();
//    CCSequence* seq = CCSequence::create(move, back,NULL);
//    _player->runAction( CCRepeatForever::create(seq));

    // 画面の表示座標をセット
    this->setViewPlayerCenter();
    this->schedule(schedule_selector(HelloWorld::setViewPlayerCenter));
//    this->schedule(schedule_selector(HelloWorld::repositionSprite));
    
    // タッチを有効化
    this->setTouchEnabled(true);
    
    return true;
}

HelloWorld::~HelloWorld()
{
//    _player->release();
}

void HelloWorld::repositionSprite(float dt)
{
    CCPoint p = _player->getPosition();
    p = CC_POINT_POINTS_TO_PIXELS(p);
    
//    std::cout << "repositionSprite" << std::endl;
//    std::cout << p.x << std::endl;
//    std::cout << p.y << std::endl;
    CCNode* map = getChildByTag(kTagTileMap);
    
    // there are only 4 layers. (grass and 3 trees layers)
    // if tamara < 81, z=4
    // if tamara < 162, z=3
    // if tamara < 243,z=2
    
    // -10: customization for this particular sample
    // ここでレイヤーの座標が触れてるかチェック
    int newZ = 6 - ( (p.y-10) / 81);
    
    std::cout << newZ << std::endl;
    newZ = fmax(newZ,0);
    
    std::cout << "repositionSprite" << std::endl;
    std::cout << newZ << std::endl;
    map->reorderChild(_player, 1);
}

void HelloWorld::setViewPlayerCenter()
{
    //    CCLog("setViewPlayerCenter");
    CCPoint playerPos = ((CCSprite *)_tileMap->getChildByTag(kTagPlayer))->getPosition();
    
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
//    CCPoint tileCoord = this->tileCoordForPosition(position);
//    int tileGid = _meta->tileGIDAt(tileCoord);
//    if (tileGid) {
//        CCDictionary *properties = _tileMap->propertiesForGID(tileGid);
//        if (properties) {
//            CCString *collision = new CCString();
//            *collision = *properties->valueForKey("Collidable");
//            if (collision && (collision->compare("true") == 0)) {
//                return;
//            }
//            
//            CCString *collectible = new CCString();
//            *collectible = *properties->valueForKey("Collectable");
//            if (collectible && (collectible->compare("true") == 0)) {
//                _meta->removeTileAt(tileCoord);
//            }
//        }
//    }
    CCLog("get touchPosition!!!");
    std::cout << position.x << std::endl;
    std::cout << position.y << std::endl;
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
    
    CCPoint tilePosition = this->getTilePosition(touchLocation);
    
    CCLog("get touchLocation!!!");
    std::cout << touchLocation.x << std::endl;
    std::cout << touchLocation.y << std::endl;
    CCLog("get tilePosition!!!");
    std::cout << tilePosition.x << std::endl;
    std::cout << tilePosition.y << std::endl;
    
    CCPoint playerPos = ((CCSprite *)_tileMap->getChildByTag(kTagPlayer))->getPosition();
    CCLog("get playerPos!!!");
    std::cout << playerPos.x << std::endl;
    std::cout << playerPos.y << std::endl;
    
//    touchLocation = CCDirector::sharedDirector()->convertToGL(touchLocation);
//    touchLocation = this->convertToNodeSpace(touchLocation);
//    
//    // 目的地
//    CCPoint destinationPos = this->getDestinationPos(touchLocation);
//    
//    // safety check on the bounds of the map
//    if (destinationPos.x <= (_tileMap->getMapSize().width * _tileMap->getTileSize().width) &&
//        destinationPos.y <= (_tileMap->getMapSize().height * _tileMap->getTileSize().height) &&
//        destinationPos.y >= 0 &&
//        destinationPos.x >= 0)
//    {
//        this->setPlayerPosition(destinationPos);
//    }
    
    return true;
}


void HelloWorld::ccTouchEnded(CCTouch *touch, CCEvent *event)
{
    CCLog("TouchEnd!");
}

CCPoint HelloWorld::getTilePosition(CCPoint point)
{
    CCTMXTiledMap* pTileMap = (CCTMXTiledMap*)this->getChildByTag(kTagTileMap);
    // タップ座標を取得
    CCPoint tilePoint = ccpSub(point, pTileMap->getPosition());
    
    // タイル幅
    float fTileWidth = pTileMap->getTileSize().width;
    // タイル高さ
    float fTileHeight = pTileMap->getTileSize().height;
    // タイル行数
    float fTileRows = pTileMap->getMapSize().height;
    // タップ座標から
    tilePoint.x = (int)(tilePoint.x / fTileWidth);
    tilePoint.y = (int)((fTileRows * fTileHeight - tilePoint.y) / fTileHeight);
    
    return tilePoint;
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
