import bpy

def add_player_start(self, context):
    bpy.ops.object.empty_add(type='CONE', align='WORLD', location=context.scene.cursor.location, scale=(1, 1, 1))
    obj = context.active_object
    obj["type"] = "player_start"
    obj.name = "Player Start"

class OBJECT_OT_add_player_start(bpy.types.Operator):
    """Adicione um cubo comum"""
    bl_idname = "mesh.add_player_start"
    bl_label = "Player Start"
    bl_options = {'REGISTER', 'UNDO'}

    text = "Player Start"
    icon='ARMATURE_DATA'

    def execute(self, context):
        add_player_start(self, context)
        return {'FINISHED'}
    
def add_entity(self, context):
    bpy.ops.object.empty_add(type='CONE', align='WORLD', location=context.scene.cursor.location, scale=(1, 1, 1))
    obj = context.active_object
    obj.name = "Entity"
    obj["type"] = "entity"
    obj["entity_type"] = "test_entity"

class OBJECT_OT_add_entity(bpy.types.Operator):
    """Adicione um cubo comum"""
    bl_idname = "mesh.add_entity"
    bl_label = "Entity"
    bl_options = {'REGISTER', 'UNDO'}

    text ="Entity"
    icon='GHOST_ENABLED'

    

    def execute(self, context):
        add_entity(self, context)
        return {'FINISHED'}
    
def add_door_triger(self, context):
    bpy.ops.mesh.primitive_cube_add(size=2, enter_editmode=False, align='WORLD', location=context.scene.cursor.location, scale=(1, 1, 1))

    obj = context.active_object
    obj["type"] = "door_triger"
    obj["key"] = ""
    obj["stay_open"] = 0
    obj["trigger_target"] = bpy.data.actions["Action"]

    obj.name = "Door Triger"
    


class OBJECT_OT_add_door_triger(bpy.types.Operator):
    """Adicione um cubo comum"""
    bl_idname = "mesh.add_door_triger"
    bl_label = "Door Triger"
    bl_options = {'REGISTER', 'UNDO'}

    text = "Door Triger"
    icon='CUBE'

    def execute(self, context):
        add_door_triger(self, context)
        return {'FINISHED'}
    
def add_sound(self, context):
    bpy.ops.object.empty_add(type='SPHERE', align='WORLD', location=context.scene.cursor.location, scale=(1, 1, 1))


    obj = context.active_object
    obj["type"] = "sound"
    obj["path"] = ""

    obj.name = "sound"
    


class OBJECT_OT_add_sound(bpy.types.Operator):
    """Adicione um cubo comum"""
    bl_idname = "mesh.add_sound"
    bl_label = "Sound"
    bl_options = {'REGISTER', 'UNDO'}

    text = "Sound"
    icon='PLAY_SOUND'

    def execute(self, context):
        add_sound(self, context)
        return {'FINISHED'}

objects_ot = [OBJECT_OT_add_player_start,OBJECT_OT_add_entity,OBJECT_OT_add_door_triger,OBJECT_OT_add_sound]

def draw_item(self, context):
    layout = self.layout
    for o in objects_ot:
        layout.operator(o.bl_idname, text=o.text)

def add_menu(self, context):
    self.layout.menu("VIEW3D_MT_ceatim_engine", text="Ceatim Engine",icon="PREFERENCES")

class VIEW3D_MT_ceatim_engine(bpy.types.Menu):
    bl_label = "Ceatim Engine"
    bl_idname = "VIEW3D_MT_ceatim_engine"

    def draw(self, context):

        for o in objects_ot:
            self.layout.operator(o.bl_idname, text=o.text,icon=o.icon)

def register():
    for o in objects_ot:
        bpy.utils.register_class(o)
    bpy.utils.register_class(VIEW3D_MT_ceatim_engine)
    bpy.types.VIEW3D_MT_add.append(add_menu)

def unregister():
    for o in objects_ot:
        bpy.utils.unregister_class(o)
    bpy.utils.unregister_class(VIEW3D_MT_ceatim_engine)
    bpy.types.VIEW3D_MT_add.remove(add_menu)

if __name__ == "__main__":
    register()
