package mindmelt.objects;

import com.opencsv.CSVReader;
import java.io.FileReader;
import java.util.ArrayList;
import java.util.List;
import java.util.Random;
import mindmelt.maps.World;

public class ObjectStore {
    static public final int PLAYER_ID = 1;
    static public final int NUM_OBJECTS = 256;
    private List<Obj> current; 
    private Obj[] objects = new Obj[NUM_OBJECTS];
    
    public void loadObjects(String filename) {
        Random rand = new Random();
        try {
            CSVReader reader = new CSVReader(new FileReader("data/"+filename+".obj"),',','\'',1);
            String line[];
            while ( (line = reader.readNext()) != null) {
                int id = Integer.parseInt(line[0]);
                String name = line[1];
                String desc = line[2];
                String type = line[3];
                int x = Integer.parseInt(line[4]);
                int y = Integer.parseInt(line[5]);
                int z = Integer.parseInt(line[6]);
                int mapId = Integer.parseInt(line[7]);
                int inside = Integer.parseInt(line[8]);
                Obj in = objects[inside];
                int icon = Integer.parseInt(line[9]);
                Obj ob = Obj.builder(type).id(id).name(name).description(desc).setCoords(x,y,z).inside(in).mapId(mapId).icon(icon);
                ob.setSpeed(500+rand.nextInt(500));
                objects[id] = ob;
            }
        } catch(Exception e) {
            e.printStackTrace();
        }
    }
    
    public void initMap(World world) {
        int mapId = world.getId();
        current = new ArrayList<Obj>();
        for (int i=1; i<NUM_OBJECTS; i++) {
            if (objects[i]==null) 
                continue;
            Obj ob = objects[i];
            if (ob.getMapId() == mapId) {
                current.add(ob);
                if (ob.isInMap())
                    ob.moveToMap(world);
            }
        }
    }
    
    public Obj getPlayer() {
        return objects[PLAYER_ID];
    }
    
    public List<Obj> getActiveObjects() {
        return current;
    }
    
    public void addToActiveObjects(Obj ob) {
        if (!ob.isInMap()) 
            return;
        if(!current.contains(ob))
            current.add(ob);
    }
}
