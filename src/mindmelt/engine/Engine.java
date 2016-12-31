package mindmelt.engine;

import mindmelt.maps.EntryExit;
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
    
    public void setWorld(World world) {
        this.world = world;
    }

    public ObjectStore getObjects() {
        return objects;
    }

    public void moveObjToMap(Obj object, int x, int y, int z) {
        object.moveToMap(x, y, z, world);
    }
    
    public boolean canEnter(Obj ob, int x, int y, int z) {
        return world.canEnter(ob, x,y,z);
    }
    
    public boolean isAnEntryExit(int x, int y, int z) {
        return world.isEntryExit(x, y, z);
    }    
    
    public EntryExit getEntryExit(int x, int y, int z) {
        return world.getEntryExit(x, y, z);
    }

}
