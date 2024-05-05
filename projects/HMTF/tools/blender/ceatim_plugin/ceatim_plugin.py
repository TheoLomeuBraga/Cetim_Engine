import bpy

def add_player_start(self, context):
    # Cria um novo mesh personalizado
    mesh = bpy.data.meshes.new("PlayerStartMesh")
    verts = [(-1, -1, 0), (1, -1, 0), (1, 1, 0), (-1, 1, 0)]  # coordenadas dos vértices
    edges = [(0, 1), (1, 2), (2, 3), (3, 0)]  # arestas conectando os vértices
    faces = [(0, 1, 2, 3)]  # face formada pelos vértices
    mesh.from_pydata(verts, edges, faces)
    
    # Cria um objeto com esse mesh
    obj = bpy.data.objects.new("Player Start", mesh)
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
    bl_label = "Player Start"
    bl_options = {'REGISTER', 'UNDO'}

    text = "Player Start"
    icon='ARMATURE_DATA'

    def execute(self, context):
        add_player_start(self, context)
        return {'FINISHED'}
    
def add_entity(self, context):
    # Cria um novo mesh personalizado
    mesh = bpy.data.meshes.new("PlayerStartMesh")
    verts = [(-1, -1, 0), (1, -1, 0), (1, 1, 0), (-1, 1, 0)]  # coordenadas dos vértices
    edges = [(0, 1), (1, 2), (2, 3), (3, 0)]  # arestas conectando os vértices
    faces = [(0, 1, 2, 3)]  # face formada pelos vértices
    mesh.from_pydata(verts, edges, faces)
        
    # Cria um objeto com esse mesh
    obj = bpy.data.objects.new("Entity", mesh)
    context.scene.collection.objects.link(obj)
        
    # Define a propriedade 'type' no objeto criado
    obj["type"] = "entity"
    obj["entity_type"] = "test_entity"
        
    # Move o objeto para a posição do cursor
    obj.location = context.scene.cursor.location

    # Define a exibição do objeto como wireframe
    obj.display_type = 'WIRE'

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

objects_ot = [OBJECT_OT_add_player_start,OBJECT_OT_add_entity]



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
