#include <cmath>
#include "CParticle.h"


// �ھڭ��O�P�ɶ��p�� delta time �������첾�t�q �A�ন�ù����첾
// ���] 2.5 �����@�Ӥ��l�q�ù��̤W�貾�ʨ�̤U��, �]�N�O���� 720 PIXEL
// 720 PIXEL = 0.5*9.8*2.5*2.5 m => 1M = 23.5102 PIXEL
// �]��Y �b���U���t, �ҥH���O�n�[�W�t��, �N����V

#define FALLING_TIME 2.5f
#define MAX_HEIGHT 720.0f
#define PIXEL_PERM (2.0f*MAX_HEIGHT/(9.8f*FALLING_TIME*FALLING_TIME))
#define GRAVITY_Y(t,dt,g) ((g)*(t+0.5f*(dt)))  //�w�g�g�L t ����A�A�g�L dt �ɶ������U�Z��

#define LIFE_NOISE(f) ((f)*(1.0f-(rand()%2001/1000.0f)))
//#define INTENSITY(f)  ( (f) >= 255 ? 255 : (f) )

inline int INTENSITY(float f) {
	if (f >= 255) return(255);
	else if (f <= 0) return(0);
	else return((int)f);
}

USING_NS_CC;

CParticle::CParticle()
{
	_fGravity = 0;
}


bool CParticle::doStep(float dt)
{
	float cost, sint;
	switch (_iType) {
	case STAY_FOR_TWOSECONDS:
		if (!_bVisible && _fElapsedTime >= _fDelayTime ) {
			_fElapsedTime = _fElapsedTime - _fDelayTime; // ���s�}�l�p��
			_bVisible = true;
			_Particle->setVisible(_bVisible);
			_Particle->setColor(_color);
			_Particle->setPosition(_Pos);
		}
		else if (_fElapsedTime> _fLifeTime) {
			_bVisible = false;
			_Particle->setVisible(_bVisible);
			return true; // ���l�ͩR�g���w�g����
		}
		else {
			sint = sinf(M_PI*_fElapsedTime / _fLifeTime);
			cost = cosf(M_PI_2*_fElapsedTime / _fLifeTime);
			_Particle->setScale(1 + sint * 2);
			_Particle->setOpacity(_fOpacity * cost);
			_Particle->setColor(Color3B(INTENSITY((_color.r + sint * 64)*(1 + sint)), 
				INTENSITY((_color.g - cost * 32)*(1 + sint)), INTENSITY((_color.b - sint * 64)*(1 + sint))));
			float tt = GRAVITY_Y(_fElapsedTime, dt, _fGravity);
			_Pos.y += (_Direction.y  * _fVelocity + tt) * dt * PIXEL_PERM;
			_Particle->setPosition(_Pos);
		}
		break;
	case RANDOMS_FALLING:
		if (!_bVisible && _fElapsedTime >= _fDelayTime) {
			_fElapsedTime = _fElapsedTime - _fDelayTime; // ���s�}�l�p��
			_bVisible = true;
			_Particle->setVisible(_bVisible);
			_Particle->setColor(_color);
			_Particle->setPosition(_Pos);
		}
		else if (_fElapsedTime > _fLifeTime) {
			_bVisible = false;
			_Particle->setVisible(_bVisible);
			return true; // ���l�ͩR�g���w�g����
		}
		else {
			sint = sinf(M_PI*_fElapsedTime / _fLifeTime);
			cost = cosf(M_PI_2*_fElapsedTime / _fLifeTime);
			_Particle->setScale(1 + sint * 1.25f);
			_Particle->setOpacity(_fOpacity * cost);
			_Particle->setColor(_color);
			_Pos.x += _Direction.x * _fVelocity * dt * PIXEL_PERM;
			float tt = GRAVITY_Y(_fElapsedTime, dt, _fGravity);
			_Pos.y += (_Direction.y * _fVelocity + tt)* dt * PIXEL_PERM;
			_Particle->setPosition(_Pos);
		}
		break;
	case FREE_FLY:
		if (!_bVisible && _fElapsedTime >= _fDelayTime) {
			_fElapsedTime = _fElapsedTime - _fDelayTime; // ���s�}�l�p��
			_bVisible = true;
			_Particle->setVisible(_bVisible);
			_Particle->setColor(_color);
			_Particle->setPosition(_Pos);
		}
		else if (_fElapsedTime > _fLifeTime) {
			_bVisible = false;
			_Particle->setVisible(_bVisible);
			return true; // ���l�ͩR�g���w�g����
		}
		else {
			sint = sinf(M_PI*_fElapsedTime / _fLifeTime);
			cost = cosf(M_PI_2*_fElapsedTime / _fLifeTime);
			_Particle->setScale(1 + sint * 2);
			_Particle->setOpacity(_fOpacity * cost);
			_Particle->setColor(_color);
			_Pos.x += _Direction.x * _fVelocity * dt * PIXEL_PERM;
			float tt = GRAVITY_Y(_fElapsedTime, dt,_fGravity);
			_Pos.y += (_Direction.y * _fVelocity + tt)* dt * PIXEL_PERM;
			//_Pos.y += _Direction.y * _fVelocity * dt;
			_Particle->setPosition(_Pos);
		}
		break;
	case EXPLOSION:
		if (!_bVisible && _fElapsedTime >= _fDelayTime) {
			_fElapsedTime = _fElapsedTime - _fDelayTime; // ���s�}�l�p��
			_bVisible = true;
			_Particle->setVisible(_bVisible);
			_Particle->setColor(_color);
			_Particle->setPosition(_Pos);
		}
		else if (_fElapsedTime > _fLifeTime) {
			_bVisible = false;
			_Particle->setVisible(_bVisible);
			return true; // ���l�ͩR�g���w�g����
		}
		else {
			sint = sinf(M_PI*_fElapsedTime / _fLifeTime);
			cost = cosf(M_PI_2*_fElapsedTime / _fLifeTime);
			_Particle->setScale(1.25 + sint*2.0);
			_Particle->setOpacity(_fOpacity * cost);
			_Particle->setColor(Color3B(INTENSITY(_color.r*(1 + sint)), INTENSITY(_color.g*(1 + sint)), INTENSITY(_color.b*(1 + sint))));
			_Pos.x += _Direction.x * cost * _fVelocity * dt * PIXEL_PERM;
			float tt = GRAVITY_Y(_fElapsedTime, dt, _fGravity);
			_Pos.y += (_Direction.y * cost * _fVelocity + tt)* dt * PIXEL_PERM;
			_Particle->setPosition(_Pos);
		}
		break;
	case HEARTSHAPE:
	case BUTTERFLYSHAPE:
		if (!_bVisible && _fElapsedTime >= _fDelayTime) {
			_fElapsedTime = _fElapsedTime - _fDelayTime; // ���s�}�l�p��
			_bVisible = true;
			_Particle->setVisible(_bVisible);
			_Particle->setColor(_color);
			_Particle->setPosition(_Pos);
		}
		else if (_fElapsedTime > _fLifeTime) {
			_bVisible = false;
			_Particle->setVisible(_bVisible);
			return true; // ���l�ͩR�g���w�g����
		}
		else {
			// �ھڤ߫������A�p��C�@�Ӥ��l���פ��m
			sint = sinf(M_PI*_fElapsedTime / _fLifeTime);
			cost = cosf(M_PI_2*_fElapsedTime / _fLifeTime);
			_Particle->setScale(1.25 + (1 - cost)*2.0);
			_Particle->setOpacity(_fOpacity * cost);
			_Particle->setColor(_color);
			_Pos.x += _Direction.x * cost * _fVelocity * dt * PIXEL_PERM;
			float tt = GRAVITY_Y(_fElapsedTime, dt, _fGravity);
			_Pos.y += (_Direction.y * cost * _fVelocity + tt)* dt * PIXEL_PERM;
			_Particle->setPosition(_Pos);
		}
		break;
	case EMITTER_DEFAULT:
		if (!_bVisible && _fElapsedTime >= _fDelayTime) {
			_fElapsedTime = 0; // ���s�}�l�p��
			_bVisible = true;
			_Particle->setVisible(_bVisible);
			_Particle->setColor(_color);
			_Particle->setPosition(_Pos);
			float degree = _fSpin * _fElapsedTime;
			_Particle->setRotation(degree);

		}
		else if (_fElapsedTime > _fLifeTime) {
			_bVisible = false;
			_Particle->setVisible(_bVisible);
			return true; // ���l�ͩR�g���w�g����
		}
		else {
			sint = sinf(M_PI*_fElapsedTime / _fLifeTime);
			cost = cosf(M_PI_2*_fElapsedTime / _fLifeTime);
			_Particle->setScale(_fSize + sint * 1.5f );
			_Particle->setOpacity(_fOpacity * cost);
			_Particle->setColor(Color3B(INTENSITY(_color.r*(1 + sint)), INTENSITY(_color.g*(1 + sint)), INTENSITY(_color.b*(1 + sint))));
//			_Particle->setColor(_color);
			_Pos.x += _Direction.x * _fVelocity * dt * PIXEL_PERM;
			float tt = GRAVITY_Y(_fElapsedTime, dt, _fGravity);
			_Pos.y += (_Direction.y * _fVelocity + tt)* dt * PIXEL_PERM;
			_Pos.y += 150 * dt;
			_Particle->setPosition(_Pos);
			
		}
		break;
	}
	float degree = _fSpin * _fElapsedTime;
	_Particle->setRotation(degree);
	// �֥[�ɶ�
	_fElapsedTime += dt;
	return false;
}


void CParticle::setBehavior(int iType)
{
	float t;
	_iType = iType;
	switch (_iType) {
	case STAY_FOR_TWOSECONDS:
		_fVelocity = 0;
		_fLifeTime = 2.5f + LIFE_NOISE(0.15f);
		_fIntensity = 1;
		_fOpacity = 255;
		_fSpin = 0;
		_fSize = 1;
		_color = Color3B(64 + rand() % 128, 64 + rand() % 128, 64 + rand() % 128);
		_fElapsedTime = 0;
		_fDelayTime = 0;
		_fGravity = 0;
		_Particle->setOpacity(255);
		_Particle->setScale(_fSize);
		break;
	case RANDOMS_FALLING:
		_fVelocity = 5.0f + rand() % 10 / 10.0f; // ��� M/Sec
		_Direction.x = 0;
		_Direction.y = -1;
		_fLifeTime = 3.0f + LIFE_NOISE(0.15f);
		_fIntensity = 1;
		_fOpacity = 255;
		_fSpin = 0;
		_fSize = 1;
		_color = Color3B(128 + rand() % 128, 128 + rand() % 128, 128 + rand() % 128);
		_fElapsedTime = 0;
		_fDelayTime = 0;
		_fGravity = 0;
		break;
	case FREE_FLY:
		_fVelocity = 5.0f + rand() % 10 / 10.0f; // ��� M/Sec
		t = 2.0f * M_PI * (rand() % 1000) / 1000.0f;
		_Direction.x = cosf(t);
		_Direction.y = sinf(t);
		_fLifeTime = 3.0f + LIFE_NOISE(0.15f);
		_fIntensity = 1;
		_fOpacity = 255;
		_fSpin = 0;
		_fSize = 1;
		_color = Color3B(128 + rand() % 128, 128 + rand() % 128, 128 + rand() % 128);
		_fElapsedTime = 0;
		_fDelayTime = 0;
		_fGravity = 0;
		break;
	case EXPLOSION:
		_fVelocity = 15.0f + rand() % 10 / 10.0f;
		t = 2.0f * M_PI * (rand() % 1000) / 1000.0f;
		_Direction.x = cosf(t);
		_Direction.y = sinf(t);
		_fLifeTime = 1.5f + LIFE_NOISE(0.15f);
		_fIntensity = 1;
		_fOpacity = 255;
		_fSpin = 0;
		_fSize = 1;
		_color = Color3B(64 + rand() % 128, 64 + rand() % 128,64 + rand() % 128);
		//_color = Color3B(255, 255, 255);
		_fElapsedTime = 0;
		_fDelayTime = rand() % 100 / 1000.0f;
		_fGravity = 0;
		break;
	case HEARTSHAPE:
		_fVelocity = 1.0f;
		t = 2.0f * M_PI * (rand() % 1000) / 1000.0f;
		float sint, cost, cos2t, cos3t, cos4t, sin12t;
		sint = sinf(t);  cost = cosf(t); cos2t = cosf(2*t); cos3t = cosf(3 * t); cos4t = cosf(4 * t);
		sin12t = sin(t/12.0f);
		_Direction.x = 16*sint*sint*sint;
		_Direction.y = 13*cost - 5*cos2t - 2*cos3t - cos4t;
		_fLifeTime = 1.5f + LIFE_NOISE(0.15f);
		_fIntensity = 1;
		_fOpacity = 255;
		_fSpin = 0;
		_fSize = 1;
		_color = Color3B(128 + rand() % 128, 128 + rand() % 128, 128 + rand() % 128);
		//_color = Color3B(255, 255, 255);
		_fElapsedTime = 0;
		_fDelayTime = rand() % 100 / 1000.0f;
		_fGravity = 0;
		break;
	case BUTTERFLYSHAPE:
		_fVelocity = 6.5f;
		t = 2.0f * M_PI * (rand() % 1000) / 1000.0f;
		sint = sinf(t);  cost = cosf(t); cos4t = cosf(4 * t); sin12t = sin(t / 12.0f);
		_Direction.x = sint*(expf(cost) - 2 * cos4t - powf(sin12t, 5));
		_Direction.y = cost*(expf(cost) - 2 * cos4t - powf(sin12t, 5));
		_fLifeTime = 1.5f + LIFE_NOISE(0.15f);
		_fIntensity = 1;
		_fOpacity = 255;
		_fSpin = 0;
		_fSize = 1;
		_color = Color3B(128 + rand() % 128, 128 + rand() % 128, 128 + rand() % 128);
		//_color = Color3B(255, 255, 255);
		_fElapsedTime = 0;
		_fDelayTime = rand() % 100 / 1000.0f;
		_fGravity = 0;
		break;
	case EMITTER_DEFAULT:
		_fIntensity = 1;
		//_fVelocity = ;
		_fOpacity = 255;
		_fSize = 1;
		_color = Color3B(rand() % 128, rand() % 128, 128 + rand() % 128);
		_fElapsedTime = 0;
		_fDelayTime = rand() % 10 / 1000.0f;
		_Particle->setScale(_fSize);
		break;
	}
}
void CParticle::setLifetime(const float lt) {
	_fLifeTime = lt + LIFE_NOISE(0.15f);;
}


void CParticle::setParticle(const char *pngName, cocos2d::Layer &inlayer)
{
	_Particle = Sprite::createWithSpriteFrameName(pngName);
	_Particle->setPosition(Point(rand() % 1024, rand() % 768));
	_Particle->setOpacity(255);
	_Particle->setColor(Color3B::WHITE);
	_bVisible = false;
	_Particle->setVisible(false);
	_iType = 0;
	//BlendFunc blendfunc = {GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA};  
	BlendFunc blendfunc = { GL_SRC_ALPHA, GL_ONE };
	_Particle->setBlendFunc(blendfunc);
	inlayer.addChild(_Particle, 1);
}

void CParticle::setVisible()
{
	_bVisible = true;
	_Particle->setVisible(_bVisible);
}

void CParticle::setPosition(const cocos2d::Point &inPos) {
	_Pos = inPos;
};

void CParticle::setGravity(const float fGravity)
{
	_fGravity = fGravity;
}

void CParticle::setParticleName(char *pngName) {
	_Particle->setSpriteFrame(pngName);
}



