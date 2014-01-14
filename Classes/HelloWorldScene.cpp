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
    
    
    CCTMXObjectGroup *objectGroup = _tileMap->objectGroupNamed("Objects");
    if (objectGroup == NULL) {
        CCLog("tile map has no objects object layer");
        return false;
    }
    
    // tmx上のSpawnPointの座標を取得
    CCDictionary *spawnPoint = objectGroup->objectNamed("SpawnPoint");
    
    int x = ((CCString)*spawnPoint->valueForKey("x")).intValue();
    int y = ((CCString)*spawnPoint->valueForKey("y")).intValue();
    
    // テクスチャアトラスからのキャラ生成
    _frameCache = CCSpriteFrameCache::sharedSpriteFrameCache();
//    CCSize size = CCDirector::sharedDirector()->getWinSize();
    _frameCache->addSpriteFramesWithFile("character.plist");
    
//    _player = CCSprite::create("Player.png");
    
    _player = CCSprite::createWithSpriteFrameName("male_walkcycle_e_01.png");
    
    
    // プレイヤーをタグ識別
    _player->setTag(kTagPlayer);
    _player->retain();
    
    // スプライトに座標セット
//    _player->setPosition(ccp(size.width/2, size.height/2));
    _player->setPosition(ccp(x,y));
    _player->setScale(1);
    _player->setAnchorPoint(ccp(0.5,0));
    
    _tileMap->addChild(_player, 1);
    
//    CCPoint playerPos = _player->getPosition();
    
    // メタレイヤー
    _meta = _tileMap->layerNamed("Meta");
    // プレイヤーの目からは見えなくする
    _meta->setVisible(false);
    
    // 画面の表示座標をセット
    this->setViewPlayerCenter();
    this->schedule(schedule_selector(HelloWorld::setViewPlayerCenter));
    
    // タッチを有効化
    this->setTouchEnabled(true);
    
    return true;
}

HelloWorld::~HelloWorld()
{
    _player->release();
}

// プレイヤーの位置をセンターに
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
    CCPoint tileCoord = this->tileCoordForPosition(position);
    CCPoint frompos = _player->getPosition();

    int tileGid = _meta->tileGIDAt(tileCoord);

    if (tileGid) {
        CCDictionary *properties = _tileMap->propertiesForGID(tileGid);
        if (properties) {
            CCString *collision = new CCString();
            *collision = *properties->valueForKey("Collidable");
            if (collision && (collision->compare("true") == 0)) {
                this->playHeroMoveAnimationFromPosition(frompos, position);
                return;
            }
        }
    }
    this->playHeroMoveAnimationFromPosition(frompos, position);
    CCMoveTo* moveTo = CCMoveTo::create(0.1f, position);
    _player->runAction(moveTo);

    std::cout << "10D10===========================" << std::endl;
    dice.roll(10, 10);
    int result = dice.getRollResult();
    dice.reset();
    std::cout << "合計値 : " << result << std::endl;

    if (this->isEncountered(result)) {
        std::cout << "敵が現れた！！" << std::endl;
    }

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
    // シングルタッチ
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
}

bool HelloWorld::ccTouchBegan(CCTouch *touch, CCEvent *event)
{
    CCLog("ccTouchBegan!");
    // タップした座標
    CCPoint touchLocation = touch->getLocationInView();

    CCPoint playerPos = ((CCSprite *)_tileMap->getChildByTag(kTagPlayer))->getPosition();

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

    CCLog("ccTouchBegan end!");
    return true;
}

void HelloWorld::ccTouchEnded(CCTouch *touch, CCEvent *event)
{
    CCLog("TouchEnd!");
}

CCPoint HelloWorld::getDestinationPos(CCPoint touchLocation)
{
    CCPoint playerPos = ((CCSprite *)_tileMap->getChildByTag(kTagPlayer))->getPosition();
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

void HelloWorld::playHeroMoveAnimationFromPosition(CCPoint fromPosition, CCPoint toPosition)
{
    char* direction = "n";
    if(toPosition.x > fromPosition.x){
        direction = "e";
    }else if(toPosition.x < fromPosition.x){
        direction = "w";
    }else if(toPosition.y < fromPosition.y){
        direction = "s";
    }
    std::cout <<  "This walkCycle" << std::endl;
    CCArray* frames = CCArray::createWithCapacity(9);
    for (int i = 0; i < 9; ++i) {

        CCString *walkCycle = CCString::createWithFormat("male_walkcycle_%s_%02d.png",direction, (i+1));

        frames->addObject(_frameCache->spriteFrameByName(walkCycle->getCString()));

        std::cout <<  walkCycle->getCString() << std::endl;
    }

    CCAnimation* animation = CCAnimation::createWithSpriteFrames(frames, 0.05f);
    CCAnimate* animate = CCAnimate::create(animation);
    _player->runAction(animate);
}
