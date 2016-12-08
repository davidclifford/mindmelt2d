package mindmelt.objects;
// basic objects

import java.util.ArrayList;
import java.util.List;
import mindmelt.maps.World;

public class Obj {
    public int id = 0;
    public int x = 0;
    public int y = 0;
    public int z = 0;
    public int mapId = 0;
    
    public Obj inside = null;
    public List<Obj> objects = null;
    
    public int icon = 0;
    public String name = "object";
    public String description = "an object";
    public String type = "object";
    
    public boolean isInMap() {
        return (mapId != 0);
    }
    
    public boolean isInObject() {
        return (inside!=null);
    }
    
    public boolean isInInventory() {
        return (inside!=null && type.equals("player"));
    }
    
    public void moveToObject(Obj obTo, World world) {
        Obj ob = this;
        if (ob.isInMap()) unlink(world);
        if (ob.isInObject()) unlink();
        List<Obj> objects = obTo.objects;
        if (objects==null) {
            objects = new ArrayList<Obj>();
        }
        objects.add(ob);
        inside = obTo;
    }
    
    public Obj moveToMap(World world) {
        world.setTop(x, y, z, this);
        return this;
    }
    
    public void moveToMap(int x, int y, int z, int mapId, World world) {
        Obj ob = this;
        if (ob.isInMap()) 
            unlink(world);
        if (ob.isInObject()) 
            unlink();
        world.setTop(x, y, z, ob);
        ob.setCoords(x, y, z);
        ob.setMapId(mapId);
    }

    private void unlink() {
        if (inside==null) 
            return;
        List<Obj> objects = inside.objects;
        objects.remove(this);
        inside = null;
    }
    
    private void unlink(World world) {
        Obj ob = this;
        List<Obj> objects = world.getObjects(ob.x, ob.y, ob.z);
        ob.setCoords(0, 0, 0);
        ob.setMapId(0);
        if(objects!=null)
            objects.remove(ob);
    }
    
    public static Obj builder() {
        Obj obj = new Obj();
        return obj;
    }
    
    public Obj setCoords(int x, int y, int z) {
        this.x = x;
        this.y = y;
        this.z = z;
        this.inside = null;
        return this;
    }  
    
    public Obj mapId(int mapId) {
        this.mapId = mapId;
        return this;
    }
    
    public Obj name(String name) {
        this.name = name;
        return this;
    }    
    
    public Obj description(String description) {
        this.description = description;
        return this;
    }
    
    public Obj type(String type) {
        this.type = type;
        return this;
    }
    
    public Obj icon(int icon) {
        this.icon = icon;
        return this;
    }    
    
    public Obj id(int id) {
        this.id = id;
        return this;
    }

    public int getId() {
        return id;
    }

    public void setId(int id) {
        this.id = id;
    }

    public int getX() {
        return x;
    }

    public void setX(int x) {
        this.x = x;
    }

    public int getY() {
        return y;
    }

    public void setY(int y) {
        this.y = y;
    }

    public int getZ() {
        return z;
    }

    public void setZ(int z) {
        this.z = z;
    }

    public int getMapId() {
        return mapId;
    }

    public void setMapId(int mapId) {
        this.mapId = mapId;
    }

    public Obj getInside() {
        return inside;
    }

    public void setInside(Obj inside) {
        this.inside = inside;
        mapId = 0;
    }

    public int getIcon() {
        return icon;
    }

    public void setIcon(int icon) {
        this.icon = icon;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public String getDescription() {
        return description;
    }

    public void setDescription(String description) {
        this.description = description;
    }

    public String getType() {
        return type;
    }

    public void setType(String type) {
        this.type = type;
    }
   
}
