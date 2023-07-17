
require("TMP_libs.definitions")
require("TMP_libs.components.component_index")

layers_table = {}
function layers_table:new_2D()
    ret = {
        main = "",
        camera = "",
        background_image = "",
        background_3D = "",
        background_cenary = "",
        cenary = "",
        charter = "",
        top = "",
        hud = "",
        sound = "",
    }
    
    function ret:create()
        self.main = create_object()
        self.camera = create_object(self.main)
        self.background_image = create_object(self.main)
        self.background_3D = create_object(self.main)
        self.background_cenary = create_object(self.main)
        self.cenary = create_object(self.main)
        self.charter = create_object(self.main)
        self.top = create_object(self.main)
        self.hud = create_object(self.main)
        self.sound = create_object(self.main)
    end
    function ret:destroy()
        remove_object(self.main)
    end
    return ret
end

function layers_table:new_3D()
    ret = {
        main = "",
        camera = "",
        background_image = "",
        background_3D = "",
        background_cenary = "",
        cenary = "",
        charter = "",
        top = "",
        hud = "",
        sound = "",
    }
    
    function ret:create()
        self.main = create_object()
        self.camera = create_object(self.main)
        self.background_image = create_object(self.main)
        self.background_3D = create_object(self.main)
        self.background_cenary = create_object(self.main)
        self.cenary = create_object(self.main)
        self.charter = create_object(self.main)
        self.top = create_object(self.main)
        self.hud = create_object(self.main)
        self.sound = create_object(self.main)
    end
    function ret:destroy()
        remove_object(self.main)
    end
    return ret
end