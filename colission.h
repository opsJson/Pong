#ifndef COLISSION_H
#define COLISSION_H

bool CheckColissionRec(Player p1, Player p2) {
	
	if (p1.x < p2.x + p2.width
    && p1.x + p1.width > p2.x
    && p1.y < p2.y + p2.height
    && p1.y + p1.height > p2.y)
    return true;
	
	return false;
}

#endif //COLISSION_H
