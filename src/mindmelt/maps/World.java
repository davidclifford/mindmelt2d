package mindmelt.maps;

import java.io.BufferedReader;
import java.io.FileReader;
import java.util.ArrayList;
import java.util.List;
import mindmelt.objects.Obj;

public class World implements ITileAccess {

    private TileType map[][][] = new TileType[8][256][256];
    private List<Obj> top[][][] = new ArrayList[8][256][256];;

    @Override
    public TileType getTile(int x, int y, int level) {
        if (x<0 || y<0) return TileType.space;
        if (x>=80 || y>=80) return TileType.space;
        return map[level][y][x];
    }     
    
    public List<Obj> getObjects(int x, int y, int level) {
        if (x<0 || y<0) return null;
        if (x>=80 || y>=80) return null;
        return top[level][y][x];
    }
    
    public Obj getTop(int x, int y, int level) {
        if (x<0 || y<0) return null;
        if (x>=80 || y>=80) return null;
        List<Obj> objects = top[level][y][x];
        if (objects==null) return null;
        return objects.get(objects.size()-1); //return last in list (ie the top)
    }    
    
    public void setTop(int x, int y, int level, Obj ob) {
        if (x<0 || y<0) return;
        if (x>=80 || y>=80) return;
        List<Obj> objects = top[level][y][x];
        if (objects==null) {
            objects = new ArrayList<Obj>();
            top[level][y][x] = objects;
        }
        objects.add(ob);
    }  
    
    public void load_map(String mapName) {
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
