#include "RenderPipeline.h"
#include <iostream>

bool RenderPipeline::init()
{
	return false;
}

void RenderPipeline::release()
{
	// place delete code here

	delete this; // yes this is safe
}

void RenderPipeline::update()
{

}

void RenderPipeline::render()
{

}


IRenderPipeline* CreateEngine()
{
	return new RenderPipeline();
}