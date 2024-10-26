#ifndef SCENETRANSITIONDEF_H
#define SCENETRANSITIONDEF_H

enum TransitionType
{
	Fade = 0,
	BowTieHoriz,
	PageCurl,
	LuminanceMelt,
	Perlin,
	WipeRight,
	WipeLeft,
	GridFlip,
	WindowSlice,
	WaterDrop,
	Doorway,
	Cube,
	MaxCnt
};

enum TransitionAction
{
	None = 0,
	Next,
	Prev
};
#endif