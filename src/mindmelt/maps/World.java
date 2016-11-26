package mindmelt.maps;

import java.io.BufferedReader;
import java.io.FileReader;

public class World implements ITileAccess {

    private TileType map[][][] = new TileType[8][256][256];

    @Override
    public TileType getTile(int x, int y, int level) {
        if (x<=0 || y<=0) return TileType.space;
        if (x>=80 || y>=80) return TileType.space;
        return map[level][y][x];
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
