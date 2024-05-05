import bpy

def add_player_start(self, context):
    # Adiciona um cubo à cena
    bpy.ops.mesh.primitive_cube_add(size=2, enter_editmode=False, location=bpy.context.scene.cursor.location)
    # Define a propriedade 'type' no objeto criado
    obj = bpy.context.active_object  # obtém o objeto recém-criado que é ativo
    obj["type"] = "player_start"  # adiciona uma propriedade personalizada chamada 'type'

class OBJECT_OT_add_player_start(bpy.types.Operator):
    """Adicione um cubo comum"""
    bl_idname = "mesh.add_player_start"
    bl_label = "Common Cube"
    bl_options = {'REGISTER', 'UNDO'}

    def execute(self, context):
        add_player_start(self, context)
        return {'FINISHED'}
#bpy.data.window_managers["WinMan"]. = 'STRING'
#bpy.data.window_managers["WinMan"]. = "type"




def draw_item(self, context):
    layout = self.layout
    layout.operator(OBJECT_OT_add_player_start.bl_idname, text="Player Start")

def add_menu(self, context):
    self.layout.menu("VIEW3D_MT_ceatim_engine", text="Ceatim Engine")

class VIEW3D_MT_ceatim_engine(bpy.types.Menu):
    bl_label = "Ceatim Engine"
    bl_idname = "VIEW3D_MT_ceatim_engine"

    def draw(self, context):
        self.layout.operator(OBJECT_OT_add_player_start.bl_idname, text="Player Start",icon='ARMATURE_DATA')

def register():
    bpy.utils.register_class(OBJECT_OT_add_player_start)
    bpy.utils.register_class(VIEW3D_MT_ceatim_engine)
    bpy.types.VIEW3D_MT_add.append(add_menu)

def unregister():
    bpy.utils.unregister_class(OBJECT_OT_add_player_start)
    bpy.utils.unregister_class(VIEW3D_MT_ceatim_engine)
    bpy.types.VIEW3D_MT_add.remove(add_menu)

if __name__ == "__main__":
    register()
