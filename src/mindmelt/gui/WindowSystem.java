package mindmelt.gui;

import java.util.ArrayList;
import java.util.List;
import mindmelt.action.Action;

public class WindowSystem extends GuiElement {

    private List<Window> windows = new ArrayList<>();
    
    public WindowSystem() {
    }

    public void draw() {
    }
    
    public void addWindow(Window window) {
        windows.add(window);
        window.setParent(this);
    }    
    
    public void deleteWindow(Window window) {
        windows.remove(window);
        window.setParent(null);
    }
    
    public void click(int x, int y, int button) {
        for(Window window : windows) {
            window.click(x,y,button);
        }        
    }
}
