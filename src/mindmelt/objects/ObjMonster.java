package mindmelt.objects;

import mindmelt.maps.World;

public class ObjMonster extends Obj {

    @Override
    public void update(World world, ObjectStore objects, int delta) {
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
            Obj topObj = world.getTopObject(dx,dy,0);
            if(world.getTile(dx, dy, 0).canEnter && (topObj==null || !world.getTopObject(dx,dy,0).isBlocked())) {
                moveToMap(dx, dy, 0, mapId, world);
                setWait(mon.getWait()-mon.getSpeed());
            }
        }
    }
}