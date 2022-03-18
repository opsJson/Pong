#ifndef RENDER_H
#define RENDER_H

static void ClearBackground(int color) {
	int i;
	for (i=0; i<screenX*screenY; i++)
		buffer[i] = color;
}

static void DrawPixel(int x, int y, int color) {
	buffer[x + (y * screenX)] = color;
}

static void DrawRec(int x, int y, int w, int h, int color) {
	int i, j;
	
	for (i=x; i<x+w; i++)
	{
		for (j=y; j<y+h; j++)
		{
			DrawPixel(i, j, color);
		}
	}
}

#endif //RENDER_H
