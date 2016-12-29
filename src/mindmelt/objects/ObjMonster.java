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
        setWait(getWait()+delta);
        if(getWait() >= getSpeed()) {
            Obj topObj = engine.getWorld().getTopObject(dx,dy,0);
            if(engine.getWorld().getTile(dx, dy, 0).canEnter() && (topObj==null || !engine.getWorld().getTopObject(dx,dy,0).isBlocked())) {
                engine.moveObjToMap(this, dx, dy, z, mapId);
                setWait(mon.getWait()-mon.getSpeed());
            }
        }
    }
}