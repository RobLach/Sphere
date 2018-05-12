#!BPY

"""
Name: 'Sphere (.drk)...'
Blender: 247
Group: 'Export'
Tooltip: 'Export as Sphere Files'
"""

__author__ = "Alexei Andreev"
__url__ = ['blender.org']
__version__ = "1.0"
__bpydoc__ = "Some source code was taken from OJB exporter by Campbell Barton and Jiri Hnidek."

import math
import Blender
from Blender import Mesh, Scene, Window, sys, Image, Draw, Armature, Mathutils
from Blender.Mathutils import *
import BPyMesh
import BPyObject
import BPySys
import BPyMessages

def fixName(name):
	if name == None:
		return 'None'
	else:
		return name.replace(' ', '_')

#Used to compare floating numbers
EPSILON = 0.000001

def write_mtl(file, materialDict, globalMaterials):

        def getShaderName(name):
                return name[name.rfind('_') + 1:]
	
	# Write material/image combinations we have used.
	for key, (mtl_mat_name, mat, img) in materialDict.iteritems():
		
		# Get the Blender data for the material and the image.
		# Having an image named None will make a bug, dont do it :)

		file.write('material %d\n' % globalMaterials[mtl_mat_name]) # Define a new material: matname_imgname
		
		if mat:
			file.write('d %.6f %.6f %.6f\n' % tuple([c*mat.ref for c in mat.rgbCol]) ) # Diffuse
			file.write('s %.6f %.6f %.6f\n' % tuple([c*mat.spec for c in mat.specCol]) ) # Specular
			file.write('h %.6f\n' % (mat.getHardness() / 10.0)) # Hardness, convert blender's (1-511) to (0.1, 51.1)
			file.write('shader %s\n' % getShaderName(mtl_mat_name))
		else:
			raise "No material defined for " + mtl_mat_name
		
		# Write images!
		if img:  # We have an image on the face!
			file.write('texture %s\n' % img.filename.split('\\')[-1].split('/')[-1]) # Diffuse mapping image			
		
		elif not mat: # No face image. if we have a material search for MTex image.
			for mtex in mat.getTextures():
				if mtex and mtex.tex.type == Blender.Texture.Types.IMAGE:
					try:
						filename = mtex.tex.image.filename.split('\\')[-1].split('/')[-1]
						file.write('texture %s\n' % filename) # Diffuse mapping image
						break
					except:
						# Texture has no image though its an image type, best ignore.
						pass

#Recursively write the bone and all its children to the file, and update the boneDictionary.
def write_bone(file, bone, boneDictionary):
        boneIndex = len(boneDictionary)
        boneDictionary[bone.name] = boneIndex
        offset = bone.length
        
        if bone.parent == None:
                parentIndex = -1
        else:
                parentIndex = boneDictionary[bone.parent.name]

        #deltaQuat is the bone's rotation
        #meshQuat is to fix mesh's rotation
        deltaQuat = bone.matrix['BONESPACE'].toQuat().normalize()
        meshQuat = bone.matrix['ARMATURESPACE'].toQuat().conjugate().normalize()
        
        file.write('bone %d %d %s.drkMesh\n' % (boneIndex, parentIndex, bone.name))
        file.write('quat %.6f %.6f %.6f %.6f\n' % tuple(deltaQuat))
        file.write('meshQuat %.6f %.6f %.6f %.6f\n' % tuple(meshQuat))
        file.write('offset %.6f\n' % offset)
        for child in bone.children:
                boneDictionary = write_bone(file, child, boneDictionary)
        return boneDictionary

#Write armature object to a file given by filePath.
def write_armature(filePath, arm_ob):

        file = open(filePath, "w")
        armature = arm_ob.data
        bones = armature.bones.values()
        file.write('bones %d\n' % len(bones))
        parentBone = bones[0]
        while parentBone.hasParent():
                parentBone = parentBone.parent

        boneDictionary = write_bone(file, parentBone, {})

        actionsDic = Blender.Armature.NLA.GetActions()
        actionNames = actionsDic.keys()
        
        file.write('actions %d\n' % len(actionNames))
        actionIndex = 0
        for actionName in sorted(actionNames):
                action = actionsDic[actionName]
                file.write('action %d\n' % actionIndex)
                file.write('frames %d\n' % (action.getFrameNumbers()[-1] - 1))#correct frame numbers to start with 0
                actionIndex += 1
                action.setActive(arm_ob)
                posBones = arm_ob.getPose().bones.values()
                ipos = action.getAllChannelIpos()
                bonesWritten = 0
                bonesString = ""
                for bone in posBones:
                        if not(bone.name in ipos):
                                continue
                        ipo = ipos[bone.name]
                        ipoLocX, ipoLocY, ipoLocZ = ipo.getCurve('LocX'), ipo.getCurve('LocY'), ipo.getCurve('LocZ')
                        ipoQuatW, ipoQuatX, ipoQuatY, ipoQuatZ = ipo.getCurve('QuatW'), ipo.getCurve('QuatX'), ipo.getCurve('QuatY'), ipo.getCurve('QuatZ')
                        keyframesCount = 0
                        animationString = ""
                        pqw, pqx, pqy, pqz = (1.0, 0.0, 0.0, 0.0)#previous quat values
                        for frame in action.getFrameNumbers():
                                qw, qx, qy, qz = ipoQuatW[frame], ipoQuatX[frame], ipoQuatY[frame], ipoQuatZ[frame]
                                quat = Quaternion(qw, qx, qy, qz).normalize()
                                pqw, pqx, pqy, pqz = qw, qx, qy, qz
                                keyframesCount += 1
                                animationString += 'frame %d\n' % (frame - 1)#correct frame numbers to start with 0
                                animationString += 'quat %.6f %.6f %.6f %.6f\n' % tuple(quat)
                                if bone.parent == None:
                                        animationString += 'off %.6f %.6f %.6f\n' % (ipoLocX[frame], ipoLocY[frame], ipoLocZ[frame])
                        if keyframesCount != 0:
                                bonesString += 'bone %d\n' % boneDictionary[bone.name]
                                bonesString += 'keyframes %d\n' % keyframesCount
                                bonesString += animationString
                                bonesWritten += 1
                file.write('bones %d\n' % bonesWritten)
                file.write(bonesString)
                
        file.close()
        Blender.Armature.NLA.GetActions()['Default'].setActive(arm_ob)
        Blender.Set("curframe", 1)

def write(directory, filename, objects):

        def v_n_uv_key(v, n, uv):
                return round(v.x, 6), round(v.y, 6), round(v.z, 6), round(n.x, 6), round(n.y, 6), round(n.z, 6), round(uv[0], 6), round(uv[1], 6)

        def v_n_key(v, n):
                return round(v.x, 6), round(v.y, 6), round(v.z, 6), round(n.x, 6), round(n.y, 6), round(n.z, 6)

        def adjust_key(key, obCenter):
                keyList = list(key)
                keyList[0] -= obCenter[0]
                keyList[1] -= obCenter[1]
                keyList[2] -= obCenter[2]
                return tuple(keyList)
	
	temp_mesh_name = '~tmp-mesh'

	scn = Scene.GetCurrent()
	
	# Get the container mesh. - used for applying modifiers and non mesh objects.
	containerMesh = meshName = tempMesh = None
	for meshName in Blender.NMesh.GetNames():
		if meshName.startswith(temp_mesh_name):
			tempMesh = Mesh.Get(meshName)
			if not tempMesh.users:
				containerMesh = tempMesh
	if not containerMesh:
		containerMesh = Mesh.New(temp_mesh_name)
		
	del meshName
	del tempMesh

        try:
                armature = Blender.Object.Get("Armature")
                write_armature(directory + filename, armature)
        except:
                armature = None
	
	# Get all meshs
	for ob_main in objects:
		for ob, ob_mat in BPyObject.getDerivedObjects(ob_main):
			me = BPyMesh.getMeshFromObject(ob, containerMesh, True, False, scn)
			if not me:
				continue

			# Initialize globalVertices and globalMaterials dictionaries
                        vertIndex = 0
                        matIndex = 0
                        globalVertices = {}
                        globalMaterials = {}
                        # Dictionary of materials: (material.name, image.name):matname_imagename
                        # matname_imagename has fixed names.
                        materialDict = {}
			
			# We have a valid mesh
			if me.faces:
				# Add a dummy object to it.
				has_quads = False
				for f in me.faces:
					if len(f) == 4:
						has_quads = True
						break
				
				if has_quads:
					oldmode = Mesh.Mode()
					Mesh.Mode(Mesh.SelectModes['FACE'])
					
					me.sel = True
					tempob = scn.objects.new(me)
					me.quadToTriangle(0) # more=0 shortest length
					oldmode = Mesh.Mode(oldmode)
					scn.objects.unlink(tempob)
					
					Mesh.Mode(oldmode)
                        else:
                                continue

			# High Quality Normals
			BPyMesh.meshCalcNormals(me)
			
			# Make our own list so it can be sorted to reduce context switching
			faces = [ f for f in me.faces ]
			faceuv = me.faceUV
			edges = me.edges

			materials = me.materials
			materialNames = []
			materialItems = materials[:]
			if materials:
				for mat in materials:
					if mat:
						materialNames.append(mat.name)
					else:
						materialNames.append(None)	
			
			# Possible there null materials, will mess up indicies
			# but at least it will export, wait until Blender gets fixed.
			materialNames.extend((16-len(materialNames)) * [None])
			materialItems.extend((16-len(materialItems)) * [None])
			
			# Sort by Material, then images
			# so we dont over context switch in the obj file.
			if faceuv:
				try:	faces.sort(key = lambda a: (a.mat, a.image, a.smooth))
				except:	faces.sort(lambda a,b: cmp((a.mat, a.image, a.smooth), (b.mat, b.image, b.smooth)))
			elif len(materials) > 1:
				try:	faces.sort(key = lambda a: (a.mat, a.smooth))
				except:	faces.sort(lambda a,b: cmp((a.mat, a.smooth), (b.mat, b.smooth)))
			else:   # no materials
				try:	faces.sort(key = lambda a: a.smooth)
				except:	faces.sort(lambda a,b: cmp(a.smooth, b.smooth))
			
			# Set the default mat to no material and no image.
			contextMat = (0, 0) # Can never be this, so we will label a new material the first chance we get.
			contextSmooth = None # Will either be true or false,  set bad to force initialization switch.
			
			name1 = ob.name
			name2 = ob.getData(1)
			obnamestring = fixName(name1)
			file = open(directory + obnamestring + ".drkMesh", "w")

                        # Fill globalVertices dictionary by creating (vert, normal, uv) tuple for all vertices of all faces
                        vertString = ""
                        obCenter = ob.getLocation()
                        if faceuv:
                                vOutputFormat = 'v %.6f %.6f %.6f %.6f %.6f %.6f %.6f %.6f\n'
                        else:
                                vOutputFormat = 'v %.6f %.6f %.6f %.6f %.6f %.6f\n'
				f_image = None
			
			#Loop through all faces
			submeshCount = 0
			faceCount = 0
			faceCounts = []
                        for face in faces:
                                if faceuv:
                                        faceUVs = list(face.uv)
                                faceUVindex = 0
                                faceIndices = []
				for v in face:
                                        if face.smooth:
                                                vno = v.no
                                        else:
                                                vno = face.no
                                        if faceuv:
                                                key = v_n_uv_key(v.co, v.no, faceUVs[faceUVindex])
                                                faceUVindex += 1
                                        else:
                                                key = v_n_key(v.co, v.no)
                                        if not globalVertices.has_key(key):
                                                globalVertices[key] = vertIndex
                                                vertString += vOutputFormat % key
                                                faceIndices.append(vertIndex)
                                                vertIndex += 1
                                        else:
                                                faceIndices.append(globalVertices[key])

                                # Make material,texture key
                                f_mat = min(face.mat, len(materialNames)-1)
                                if faceuv:
					f_image = face.image
				if faceuv and f_image:
					matKey = materialNames[f_mat],  f_image.name
				else:
					matKey = materialNames[f_mat],  None

				# Check for context switch
				if matKey != contextMat:
                                        submeshCount += 1
					if matKey[0] == None and matKey[1] == None:
						# Write a null material, since we know the context has changed.
						faceString += 'use (null)\n' # mat, image
						
					else:
						mat_data= materialDict.get(matKey)
						if not mat_data:
							mat_data = materialDict[matKey] = fixName(matKey[0]), materialItems[f_mat], f_image
						
						vertString += 'use %d\n' % matIndex
						globalMaterials[mat_data[0]] = matIndex
						matIndex += 1
					if faceCount != 0:
                                                faceCounts.append(faceCount)
                                                faceCount = 0
					
				contextMat = matKey
                                vertString += 'face %d %d %d\n' % tuple(faceIndices)
                                faceCount += 1
                        faceCounts.append(faceCount)
                        file.write('count %d\n' % vertIndex)
                        if faceuv:
                                file.write('uvs\n')
                        file.write('submeshes %d\n' % submeshCount)
                        for faceCount in faceCounts:
                                file.write('faces %d\n' % faceCount)
			file.write(vertString)

			me.verts = None
			write_mtl(file, materialDict, globalMaterials)
                        file.close()

	
def write_ui(filePath):

        Window.EditMode(0)
        slashPos = filePath.rfind('\\')
	directory = filePath[:slashPos + 1]
	filename = filePath[slashPos + 1:]

	if not filename.lower().endswith('.drk'):
		filename += '.drk'
	
	if not BPyMessages.Warning_SaveOver(filePath):
		return

	context = Scene.GetCurrent().getRenderingContext()
	orig_frame = Blender.Get('curframe')
	Blender.Set('curframe', context.startFrame())
	write(directory, filename, Scene.GetCurrent().objects)
	Blender.Set('curframe', orig_frame)
	
	# Restore old active scene.
	Window.WaitCursor(0)


if __name__ == '__main__':
	Window.FileSelector(write_ui, 'Export Darkness Drk', sys.makename(ext='.drk'))
