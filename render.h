#ifndef RENDER_H
#define RENDER_H

void ClearBackground(int color) {
	int i;
	for (i=0; i<screenX*screenY; i++)
		buffer[i] = color;
}

static void DrawPixel(int x, int y, int color) {
	buffer[x + (y * screenX)] = color;
}

void DrawRec(int x, int y, int w, int h, int color) {
	int x1, y1;
	
	for (y1=x; y1<x+w; y1++)
	{
		for (x1=y; x1<y+h; x1++)
		{
			DrawPixel(y1, x1, color);
		}
	}
}

#endif //RENDER_H
