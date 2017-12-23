#include "CParticleSystem.h"

#define NUMBER_PARTICLES 1000 // 預設一次取得 1000 個 Particles

USING_NS_CC;

CParticleSystem::CParticleSystem()
{
	_fGravity = 0;
	_bEmitterOn = false;
}

void CParticleSystem::setEmitter(bool bEm)
{
	_bEmitterOn = bEm;
}

void CParticleSystem::init(cocos2d::Layer &inlayer)
{
	_iFree = NUMBER_PARTICLES;
	_iInUsed = 0;
	_pParticles = new CParticle[NUMBER_PARTICLES]; // 取得所需要的 particle 空間
	 // 讀入儲存多張圖片的 plist 檔
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("particletexture.plist");
	for (int i = 0; i < NUMBER_PARTICLES; i++) {
		_pParticles[i].setParticle("flare.png", inlayer);
		_FreeList.push_front(&_pParticles[i]);
	}
}

void CParticleSystem::doStep(float dt)
{
	CParticle *get;
	list <CParticle *>::iterator it;
	if (_bEmitterOn) { // 根據 Emitter 設定的相關參數，產生相對應的分子
					   // 先計算在累加
		switch (_iType) {
			case EMITTER_DEFAULT:
				if (_iFree != 0) {
					int n = (int)(_fElpasedTime * _iNumParticles); // 到目前為止應該產生的分子個數
					if (n > _iGenParticles) {  // 產生的分子個數不足，產生到 n 個分子
						for (int i = 0; i < n - _iGenParticles; i++) {
							// 根據 Emitter 的相關參數，設定所產生分子的參數
							if (_iFree != 0) {
								get = _FreeList.front();
								get->setBehavior(EMITTER_DEFAULT);
								get->setVelocity(_fVelocity);
								get->setLifetime(_fLifeTime);
								get->setGravity(_fGravity);
								get->setPosition(_emitterPt);
								get->setColor(Color3B(_fRed, _fGreen, _fBlue));
								get->setSpin(_fSpin);
								get->setOpacity(_fOpacity);
								get->setSize(0.125f);
								get->setParticleName(_cName);
								get->setWindDir(_fWindDir);
								get->setWindStr(_fWindStr);
								// 根據 _fSpread 與 _vDir 產生方向
								float t = (rand() % 1001) / 1000.0f; // 產生介於 0 到 1 間的數
								t = _fSpread - t * _fSpread * 2; //  產生的角度，轉成弧度
								t = (_fDir + t)* M_PI / 180.0f;
								Vec2 vdir(cosf(t), sinf(t));
								get->setDirection(vdir);
								_FreeList.pop_front();
								_InUsedList.push_front(get);
								_iFree--; _iInUsed++;
							}
						}
						_iGenParticles = n; // 目前已經產生 n 個分子

						if (_fElpasedTime >= 1.0f) {
							_fElpasedTime -= 1.0f;
							if (_iGenParticles >= _iNumParticles) _iGenParticles -= _iNumParticles;
							else _iGenParticles = 0;
						}

					}
				}
				break;
			case FIREWORK:
				if (_iFree != 0) {
					if (em.y < _emitterPt.y) {
						for (int i = 0; i < 55; i++) {
							// 根據 Emitter 的相關參數，設定所產生分子的參數
							if (_iFree != 0) {
								get = _FreeList.front();
								get->setBehavior(EMITTER_DEFAULT);
								get->setVelocity(3.5);
								get->setLifetime(0.5);
								get->setGravity(-1.4);
								get->setPosition(em);
								get->setColor(Color3B(255, 75, 75));
								get->setSpin(0);
								get->setOpacity(255);
								get->setSize(0.125f);
								get->setParticleName("spark.png");
								get->setWindDir(0);
								get->setWindStr(0);
								// 根據 _fSpread 與 _vDir 產生方向
								float t = (rand() % 1001) / 1000.0f; // 產生介於 0 到 1 間的數
								t = 14.5f - t * 14.5f * 2; //  產生的角度，轉成弧度
								t = (270 + t)* M_PI / 180.0f;
								Vec2 vdir(cosf(t), sinf(t));
								get->setDirection(vdir);
								_FreeList.pop_front();
								_InUsedList.push_front(get);
								_iFree--; _iInUsed++;
							}
						}
						em.y += 6;
					}
					else {
						if (_iFree > 100) {
							for (int i = 0; i < 100; i++) {
								get = _FreeList.front();
								get->setBehavior(EXPLOSION);
								get->setPosition(em);
								get->setGravity(_fGravity);
								get->setParticleName("spark.png");
								_FreeList.pop_front();
								_InUsedList.push_front(get);
								_iFree--; _iInUsed++;
							}
							em = _emitterPt - Vec2(0, 450);
						}
					}
				}
				break;
			case ROLL:		
				if (_iFree != 0) {
					float t = _totalTime* M_PI / 180.0f * 750;
					vdir = Vec2(cosf(t), sinf(t));
					em = _emitterPt + 50 * vdir;
						// 根據 Emitter 的相關參數，設定所產生分子的參數
						if (_iFree != 0) {
							get = _FreeList.front();
							get->setBehavior(ROLL);
							get->setVelocity(_fVelocity);
							get->setLifetime(_fLifeTime);
							get->setGravity(_fGravity);
							get->setPosition(em);
							get->setColor(Color3B(rand() % 255 + 1, rand() % 255 + 1, rand() % 255 + 1));
							get->setSpin(_fSpin);
							get->setOpacity(_fOpacity);
							get->setSize(0.125f);
							get->setParticleName(_cName);
							get->setWindDir(_fWindDir);
							get->setWindStr(_fWindStr);
							// 根據 _fSpread 與 _vDir 產生方向
							float t = (rand() % 1001) / 1000.0f; // 產生介於 0 到 1 間的數

							get->setDirection(vdir);
							_FreeList.pop_front();
							_InUsedList.push_front(get);
							_iFree--; _iInUsed++;
						}
				}
				break;
			case WATERBALL:
				if (_iFree != 0) {
					float t = _totalTime * M_PI / 180.0f * 1000;
					Vec2 vfdir = Vec2(cosf(t), sinf(t));
					em = _emitterPt + 150 * vfdir;
					for (int i = 0; i < 10; i++) {
						// 根據 Emitter 的相關參數，設定所產生分子的參數
						if (_iFree != 0) {
							get = _FreeList.front();
							get->setBehavior(WATERBALL);
							get->setVelocity(_fVelocity * 5);
							get->setLifetime(0.5f);
							get->setGravity(_fGravity);
							get->setPosition(em);
							get->setColor(Color3B(rand() % 255 + 1, rand() % 255 + 1, rand() % 255 + 1));
							get->setSpin(_fSpin);
							get->setOpacity(_fOpacity);
							get->setSize(0.05f);
							get->setEm(_emitterPt);
							get->setParticleName(_cName);
							get->setRDelayTime(0.05f*i);
							// 根據 _fSpread 與 _vDir 產生方向
							get->setDirection(vfdir);
							_FreeList.pop_front();
							_InUsedList.push_front(get);
							_iFree--; _iInUsed++;
						}


					}
				}
				break;
		}

		_fElpasedTime += dt;
		_totalTime += dt;
	}

	if (_iInUsed != 0) { // 有分子需要更新時
		for (it = _InUsedList.begin(); it != _InUsedList.end(); ) {
			if ((*it)->doStep(dt)) { // 分子生命週期已經到達
									 // 將目前這一個節點的內容放回 _FreeList
				_FreeList.push_front((*it));
				it = _InUsedList.erase(it); // 移除目前這一個, 
				_iFree++; _iInUsed--;
			}
			else it++;
		}
	}

}

void CParticleSystem::setGravity(float fGravity)
{
	// 設定目前 particle 是在 inused 的 gravity
	_fGravity = fGravity;
	list <CParticle *>::iterator it;
	if (_iInUsed != 0) { // 有分子需要更新時
		for (it = _InUsedList.begin(); it != _InUsedList.end(); it++) {
			(*it)->setGravity(_fGravity);
		}
	}
}

void CParticleSystem::setParticlesName(char *c)
{
	_cName = c;
	for (int i = 0; i < NUMBER_PARTICLES; i++) {
		_pParticles[i].setParticleName(_cName);

	}
}

CParticleSystem::~CParticleSystem()
{
	if (_iInUsed != 0) _InUsedList.clear(); // 移除所有的 NODE
	if (_iFree != 0) _FreeList.clear();
	delete[] _pParticles; // 釋放所有取得資源
	SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("particletexture.plist");
}

void CParticleSystem::onTouchesBegan(const cocos2d::CCPoint &touchPoint)
{
	CParticle *get;
	switch (_iType)
	{
	case STAY_FOR_TWOSECONDS:
		// 從 _FreeList 取得一個分子給放到 _InUsed
		if (_iFree != 0) {
			get = _FreeList.front();
			get->setBehavior(STAY_FOR_TWOSECONDS);
			get->setPosition(touchPoint);
			get->setGravity(_fGravity);
			_FreeList.pop_front();
			_InUsedList.push_front(get);
			_iFree--; _iInUsed++;
		}
		else return;// 沒有分子, 所以就不提供
		break;
	case RANDOMS_FALLING :
		if (_iFree != 0) {
			get = _FreeList.front();
			get->setBehavior(RANDOMS_FALLING);
			get->setPosition(touchPoint);
			get->setGravity(_fGravity);
			_FreeList.pop_front();
			_InUsedList.push_front(get);
			_iFree--; _iInUsed++;
		}
		else return;// 沒有分子, 所以就不提供
		break;
	case FREE_FLY:
		// 從 _FreeList 取得一個分子給放到 _InUsed
		if (_iFree != 0) {
			get = _FreeList.front();
			get->setBehavior(FREE_FLY);
			get->setPosition(touchPoint);
			get->setGravity(_fGravity);
			_FreeList.pop_front();
			_InUsedList.push_front(get);
			_iFree--; _iInUsed++;
		}
		else return;// 沒有分子, 所以就不提供
		break;
	case EXPLOSION:
		// 從 _FreeList 取得一個分子給放到 _InUsed
		if (_iFree > 100) {
			for (int i = 0; i < 100; i++) {
				get = _FreeList.front();
				get->setBehavior(EXPLOSION);
				get->setPosition(touchPoint);
				get->setGravity(_fGravity);
				_FreeList.pop_front();
				_InUsedList.push_front(get);
				_iFree--; _iInUsed++;
			}
		}
		else return;// 沒有分子, 所以就不提供
		break;
	case HEARTSHAPE:
		// 從 _FreeList 取得一個分子給放到 _InUsed
		if (_iFree > 150) {
			for (int i = 0; i < 150; i++) {
				get = _FreeList.front();
				get->setBehavior(HEARTSHAPE);
				get->setPosition(touchPoint);
				get->setGravity(_fGravity);
				_FreeList.pop_front();
				_InUsedList.push_front(get);
				_iFree--; _iInUsed++;
			}
		}
		else return;// 沒有分子, 所以就不提供
		break;
	case BUTTERFLYSHAPE:
		// 從 _FreeList 取得一個分子給放到 _InUsed
		if (_iFree > 150) {
			for (int i = 0; i < 150; i++) {
				get = _FreeList.front();
				get->setBehavior(BUTTERFLYSHAPE);
				get->setPosition(touchPoint);
				get->setGravity(_fGravity);
				_FreeList.pop_front();
				_InUsedList.push_front(get);
				_iFree--; _iInUsed++;
			}
		}
		else return;// 沒有分子, 所以就不提供
		break;
	}
}

void CParticleSystem::onTouchesMoved(const cocos2d::CCPoint &touchPoint)
{
	CParticle *get;
	switch (_iType)
	{
	case STAY_FOR_TWOSECONDS:
		// 從 _FreeList 取得一個分子給放到 _InUsed
		if (_iFree != 0) {
			get = _FreeList.front();
			get->setBehavior(STAY_FOR_TWOSECONDS);
			get->setPosition(touchPoint);
			get->setGravity(_fGravity);
			_FreeList.pop_front();
			_InUsedList.push_front(get);
			_iFree--; _iInUsed++;
		}
		else return;// 沒有分子, 所以就不提供
		break;
	case RANDOMS_FALLING:
		if (_iFree != 0) {
			get = _FreeList.front();
			get->setBehavior(RANDOMS_FALLING);
			get->setPosition(touchPoint);
			get->setGravity(_fGravity);
			_FreeList.pop_front();
			_InUsedList.push_front(get);
			_iFree--; _iInUsed++;
		}
		else return;// 沒有分子, 所以就不提供
		break;
	case FREE_FLY:
		// 從 _FreeList 取得一個分子給放到 _InUsed
		if (_iFree != 0) {
			get = _FreeList.front();
			get->setBehavior(FREE_FLY);
			get->setPosition(touchPoint);
			get->setGravity(_fGravity);
			_FreeList.pop_front();
			_InUsedList.push_front(get);
			_iFree--; _iInUsed++;
		}
		else return;// 沒有分子, 所以就不提供
		break;
	}
}