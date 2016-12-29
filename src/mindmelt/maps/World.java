package mindmelt.maps;

import java.io.BufferedReader;
import java.io.FileReader;
import java.util.ArrayList;
import java.util.List;
import mindmelt.objects.Obj;

public class World implements ITileAccess {

    public static final int MAP_SIZE = 80;
    public static final int LAYERS = 8;
    private TileType map[][][] = new TileType[LAYERS][MAP_SIZE][MAP_SIZE];
    private List<Obj> top[][][] = new ArrayList[LAYERS][MAP_SIZE][MAP_SIZE];
    ;

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
        String filename = "st/"+mapName+".st";
        
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
}
