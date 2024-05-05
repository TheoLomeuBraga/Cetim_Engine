import bpy

def add_player_start(self, context):
    # Cria um novo mesh personalizado
    mesh = bpy.data.meshes.new("PlayerStartMesh")
    verts = [(-1, -1, 0), (1, -1, 0), (1, 1, 0), (-1, 1, 0)]  # coordenadas dos vértices
    edges = [(0, 1), (1, 2), (2, 3), (3, 0)]  # arestas conectando os vértices
    faces = [(0, 1, 2, 3)]  # face formada pelos vértices
    mesh.from_pydata(verts, edges, faces)
    
    # Cria um objeto com esse mesh
    obj = bpy.data.objects.new("Player_Start", mesh)
    context.scene.collection.objects.link(obj)
    
    # Define a propriedade 'type' no objeto criado
    obj["type"] = "player_start"
    
    # Move o objeto para a posição do cursor
    obj.location = context.scene.cursor.location

    # Define a exibição do objeto como wireframe
    obj.display_type = 'WIRE'

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
    self.layout.menu("VIEW3D_MT_ceatim_engine", text="Ceatim Engine",icon="PREFERENCES")

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
