package mindmelt.objects;

import mindmelt.engine.Engine;
import mindmelt.maps.World;

public class ObjMonster extends Obj {

    @Override
    public void update(Engine engine, int delta) {
        ObjMonster mon = this;
        int dx = x;
        int dy = y;
        if (rand.nextInt(2)==0) {
            dx += rand.nextInt(2)*2-1;
        } else {
            dy += rand.nextInt(2)*2-1;
        }
        if(isReady(delta)) {
            if(engine.canEnter(this,dx,dy,z)) {
                engine.moveObjToMap(this, dx, dy, z);
            }
        }
    }
}