#ifndef _SCENE_H_
#define _SCENE_H_

class BaseScene
{
public:
	virtual void Init() = 0;
	virtual void Uninit() = 0;
	virtual void Update() = 0;
	virtual void Render() = 0;
};

#endif //!_SCENE_H_
