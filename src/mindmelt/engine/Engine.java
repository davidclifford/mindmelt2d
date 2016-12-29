package mindmelt.engine;

import mindmelt.maps.World;
import mindmelt.objects.Obj;
import mindmelt.objects.ObjectStore;

public class Engine {
    
    private World world;
    private ObjectStore objects;
    
    public Engine(World world, ObjectStore objects) {
        this.world = world;
        this.objects = objects;
    }

    public World getWorld() {
        return world;
    }

    public ObjectStore getObjects() {
        return objects;
    }

    public void moveObjToMap(Obj object, int x, int y, int z, int mapId) {
        object.moveToMap(x, y, z, mapId, world);
    }

}
