package mindmelt.objects;
// basic objects

import java.util.ArrayList;
import java.util.List;


public class Obj {
    public int id = 0;
    public int x = 0;
    public int y = 0;
    public int z = 0;
    public int mapId = 0;
    public Obj inside = null;
    public int icon = 0;
    public String name = "object";
    public String description = "an object";
    public String type = "object";
    
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
    
    public Obj moveInto(Obj inside) {
        this.x = 0;
        this.y = 0;
        this.z = 0;
        this.inside = inside;
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
