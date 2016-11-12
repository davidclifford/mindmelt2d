package mindmelt.maps;

import java.awt.Color;

public class TileType {
    public int id = 0;
    public int icon = 0;
    public String name = "space";
    public boolean canEnter = false;
    public boolean seeThru = false;
    public int speed = 1;
    public int ch = '$';
    public Color color = Color.BLACK;
    
    public static TileType tileType[] = new TileType[256];

    public static final TileType space = new TileType(0).name("space");
    public static final TileType floor = new TileType(1).name("floor").enter().seeThru().ch(' ').icon(1);
    public static final TileType presurepad = new TileType(2).name("pressure pad").enter().seeThru().ch('o').icon(2);
    public static final TileType hiddenpp = new TileType(3).name("hidden presure pad").enter().seeThru().ch('.').icon(3);
    public static final TileType hiddentele = new TileType(4).name("hidden teleport").enter().seeThru().ch('t').icon(4);
    public static final TileType leftturn = new TileType(5).name("left turn").enter().seeThru().ch(')').icon(5);
    public static final TileType rightturn = new TileType(6).name("right turn").enter().seeThru().ch('(').icon(6);
    public static final TileType uturn = new TileType(7).name("U turn").enter().seeThru().ch('U').icon(7);
    public static final TileType hiddenff = new TileType(8).name("hidden force field").seeThru().ch('z').icon(8);
    public static final TileType pit = new TileType(9).name("pit").enter().seeThru().ch('X').icon(9);
    public static final TileType wall = new TileType(10).name("wall").ch('#').icon(10);

    
    public TileType(int id) {
        if (tileType[id] != null) {
            System.out.println("Tile already registered");
            System.exit(1);
        }
        tileType[id] = this;
        this.id = id;
    }

    public TileType icon(int icon) {
        this.icon = icon;
        return this;
    }
    public TileType name(String name) {
        this.name = name;
        return this;
    }
    public TileType enter() {
        canEnter = true;
        return this;
    }
    public TileType seeThru() {
        this.seeThru = true;
        return this;
    }
    public TileType speed(int speed) {
        this.speed = speed;
        return this;
    }
    public TileType ch(char c) {
        ch = c;
        return this;
    }
    public TileType color(Color color) {
        this.color = color;
        return this;
    }

}
