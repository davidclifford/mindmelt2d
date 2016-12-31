package mindmelt.maps;

import java.io.BufferedReader;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import mindmelt.objects.Obj;

public class World implements ITileAccess {

    public static final int MAP_SIZE = 80;
    public static final int LAYERS = 8;
    private TileType map[][][] = new TileType[LAYERS][MAP_SIZE][MAP_SIZE];
    private List<Obj> top[][][] = new ArrayList[LAYERS][MAP_SIZE][MAP_SIZE];
    private List<Area> nomonsters = new ArrayList<>();
    
    class Coord {
        int x;
        int y;
        int z;
        
        Coord(int x, int y, int z) {
            this.x = x;
            this.y = y;
            this.z = z;
        }
    }
    
    class Area {
        Coord topleft;
        Coord botright;
        
        Area(int x1, int y1, int z1, int x2, int y2, int z2) {
            this.topleft = new Coord(x1,y1,z1);
            this.botright = new Coord(x2,y2,z2);
        }       
        
        Area(List<Integer> coords) {
            this.topleft = new Coord(coords.get(0),coords.get(1),coords.get(2));
            this.botright = new Coord(coords.get(3),coords.get(4),coords.get(5));
        }
    }

    @Override
    public TileType getTile(int x, int y, int level) {
        if (x<0 || y<0) return TileType.space;
        if (x>=MAP_SIZE || y>=MAP_SIZE) return TileType.space;
        return map[level][y][x];
    }     
    
    public List<Obj> getObjects(int x, int y, int level) {
        if (x<0 || y<0) return null;
        if (x>=MAP_SIZE || y>=MAP_SIZE) return null;
        return top[level][y][x];
    }
    
    public Obj getTopObject(int x, int y, int level) {
        if (x<0 || y<0) return null;
        if (x>=MAP_SIZE || y>=MAP_SIZE) return null;
        List<Obj> objects = top[level][y][x];
        if (objects==null || objects.size()==0) return null;
        return objects.get(objects.size()-1); //return last in list (ie the top)
    }    
    
    public void setTop(int x, int y, int level, Obj ob) {
        if (x<0 || y<0) return;
        if (x>=MAP_SIZE || y>=MAP_SIZE) return;
        List<Obj> objects = top[level][y][x];
        if (objects==null) {
            objects = new ArrayList<Obj>();
            top[level][y][x] = objects;
        }
        objects.add(ob);
    }  
    
    public void removeObject(Obj ob) {
        List<Obj> objects = getObjects(ob.x, ob.y, ob.z);
        ob.setCoords(0, 0, 0);
        ob.setMapId(0);
        if(objects!=null) {
            objects.remove(ob);
            if (objects.isEmpty())
                top[ob.x][ob.y][ob.z] = null;
        }
    }
    
    public void loadMap(String mapName) {
        loadControl(mapName);
        loadWorld(mapName);
    }
    
    private String readLine(BufferedReader input) {
        String line;
        
        try {
            while ((line = input.readLine()) != null) {
               if (line.startsWith("//")) continue;
               return line;
            }   
        } catch(Exception e) {
            System.out.println(e.toString());
        }
        return null;
    }
        
    public void loadControl(String mapName) {
        String filename = "data/"+mapName+".control";
        String line = "";
        BufferedReader input = null;
        try {
            input = new BufferedReader(new FileReader(filename));
        } catch (FileNotFoundException e) {
            System.out.println("Map "+mapName+" not found");
        }
        
        while ((line = readLine(input)) != null) {
            if (line.startsWith("@")) {
                String section = line.substring(1);
                switch (section) {
                    case "map":
                        readHeader(input);
                        break;
                    case "entry":
                        readEntry(input); 
                        break;
                    case "control":
                        readControl(input);
                        break;
                    case "talk":
                        readTalk(input);
                        break;
                    default: 
                        System.out.println("Unsupported control section @"+section);
                }
            }
        }
    }   
    
    private String[] keyValue(String line) {
        String words[] = line.split("=");
        return words;
    }
    
    private List<Integer> numbers(String value) {
        String values[] = value.split(",");
        List<Integer> vals = new ArrayList<>();
        for(String val : Arrays.asList(values)) {
            vals.add(new Integer(val));
        }
        return vals;
    }
    
    private void readHeader(BufferedReader input) {
        String line="";
        String key="";
        String value="";
        while( (line=readLine(input))!= null) {
            if(line.startsWith("--"))
                return;
            String kv[] = keyValue(line);
            switch(kv[0]) {
                case "nomonster" :
                    List<Integer> area = numbers(kv[1]);
                    nomonsters.add(new Area(area));
            }
        }
    }
    
    private void readEntry(BufferedReader input) {
        String line="";
        while( (line=readLine(input))!= null) {
            if(line.startsWith("--"))
                return;
        }
    }
    
    private void readControl(BufferedReader input) {
        String line="";
        while( (line=readLine(input))!= null) {
            if(line.startsWith("--"))
                return;
        }
    }
    
    private void readTalk(BufferedReader input) {
        String line="";
        while( (line=readLine(input))!= null) {
            if(line.startsWith("--"))
                return;
        }        
    }
    
    private void loadWorld(String mapName) {
        String filename = "data/"+mapName+".map";
        
        try {
            BufferedReader input = new BufferedReader(new FileReader(filename));
            //Header
            int level = 0;

            String line;
            int y=0;
            while ((line = input.readLine())!=null) {
                int x=0;
                for(int c=0; c<line.length();c++) {
                    char ch = line.charAt(c);
                    TileType tile = TileType.getFromChar(ch);
                    map[level][y][x] = tile;
                    x++;
                }
                y++;
            }
        } catch (Exception e)   {
            System.out.println(e.toString());
        }
    }
    
    public boolean canEnter(Obj ob, int x, int y, int z) {
        if (getTile(x,y,z).canEnter()) {
            if(getTopObject(x,y,z)== null || !getTopObject(x,y,z).isBlocked()) {
                if(ob.isMonster() && inNoMonsterArea(x,y,z)) {
                    return false;
                } else {
                    return true;        
                }
            }
        }
        return false;
    }
    
    private boolean inNoMonsterArea(int x, int y, int z) {
        for(Area a : nomonsters) {
            if (a.topleft.z == z && a.topleft.x <= x && a.topleft.y <= y && a.botright.x >= x && a.botright.y >= y) {
                return true;
            }
        }
        return false;
    }
}
