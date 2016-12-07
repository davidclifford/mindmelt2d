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
    public Obj up = null;
    public Obj down = null;
    public Obj top = null;
    
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
    
    public boolean isAtTop() {
        return (this.up == null);
    }
    
    public void moveToObject(Obj obTo) {
        Obj ob = this;
        unlink();
        ob.inside = obTo;
        if(obTo.top!=null) {
            ob.down = obTo.top;
            ob.up = null;
            Obj dob = obTo.top;
            obTo.top = ob;
            dob.up = ob;            
        }
    }
    
    public void moveToMap(int x, int y, int z, int mapId, World world) {
        Obj ob = this;
        unlink();
        Obj oldTop = world.getTop(x, y, z);
        if (oldTop!=null) {
            ob.down = oldTop;
            oldTop.up = ob;
        }
        world.setTop(x,y,z,ob);
        ob.setCoords(x, y, z);
        ob.setMapId(mapId);
    }
    
    private void unlink() {
        Obj ob = this;
        if(ob.isInMap()) {
            ob.setCoords(0, 0, 0);
            ob.setMapId(0);
        } else if (ob.isInObject()) {
            Obj inside = ob.getInside();
            if (inside.top == ob) {
                inside.top = ob.down;
            }
        }
        
        if (ob.up!=null) {
            ob.up.down = ob.down;
        }
        if (ob.down!=null) {
            ob.down.up = ob.up;
        }
        ob.up = null;
        ob.down = null;
        ob.inside = null;
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
