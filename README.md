# Spectral-Based-Mesh-Segmentation
Spectral Based Mesh Segmentation build on top of a custom simple self-made OpenGL framework

A Graph Theory course term project.
Inspired from this paper:

`Mejia, D., Ruiz-Salguero, O., & Cadavid, C. A. (2017). Spectral-based mesh segmentation. International Journal on Interactive Design and Manufacturing (IJIDeM), 11, 503-514.`

A simple mesh segmentation framework relies on the Laplacian Matrix. 
Scroll down to see results

# How to use?
You can either clone the project and use build/Spectral_Based_Mesh_Segmentation.sln by configuring necessary library directions (Eigen, Spectra, GLFW, GLEW) or configure CMakeList.txt with your own paths to mentioned librarires to create a solution file.

(Note that OpenGL stuff, GLFW and GLEW, is noT mandatory, you can simply extract Mesh.h and Mesh.cpp with other dependencies (SVertex.h and STriangle.h) and use them independently on your own projects but obviously you need Eigen and Spectra too)

Once you finished the configuration phase (congrats by the way!); the usage is simple.
```c++
    Mesh myMesh; //create mesh object
    myMesh.loadOFF("C:/Cpp_Projects/Spectral_Based_Mesh_Segmentation/3DObjects/125.off"); //Load your 3D Object
    myMesh.segment(5); //call segment method with the desired segmentation count
```
If you want to make use of the simple OpenGL renderer I've created for this project, you can follow these steps:
```c++
    Window window(1280, 1024, "My Window"); //Create a window

    if (glewInit() != GLEW_OK) { //check if everything is OK
        printf("GLEW initialisation failed!");
        glfwDestroyWindow(window.m_Window);
        glfwTerminate();
        return -1;
    }
    Mesh myMesh;
    myMesh.loadOFF("C:/Cpp_Projects/Spectral_Based_Mesh_Segmentation/3DObjects/125.off");

    Camera camera(myMesh.getBestCamPos().value(), glm::vec3(0.0f, 1.0f, 0.0f)); // create a camera and make it look to your mesh
    // Load a shader program
    Shader myShader;
    myShader.Generate("C:/Cpp_Projects/Spectral_Based_Mesh_Segmentation/Shaders/vertexShader.glsl", "C:/Cpp_Projects/Spectral_Based_Mesh_Segmentation/Shaders/fragmentShader.glsl"); //load your shaders so that we can paint stuff
    
    MeshRenderer meshRenderer(myMesh, myShader); //create mesh renderer to link your mesh data with shaders
    myMesh.segment(5); //segment your mesh (you can do it anywhere after loading your 3d object)
    meshRenderer.PaintVertices(); //with the segmentation, you have colored vertex data and you can use it
    meshRenderer.Render(window,camera); //the almighty render loop!
    
```
As you can see, this repo is very open for future improvements (for example, Mesh class can write to the 3D object file the colors of vertices) and you are always welcome to do so!

# How it works?
If you want to dive deep on the topic you definitely should read the [paper][MyPaper] I uploaded. To summarize, we are creating the Laplacian Matrix of the mesh and 
then we are finding its second smallest Eigenvalue and corresponding Eigenvector, thanks to amazing [Eigen][EigenLib] and [Spectra][SpectraLib] libraries,we can
store sparse matrices (w/Eigen), which results in efficient memory usage, and can calculate only the first two smallest eigenvectors (w/Spectra) by avoiding spending time on trivial Eigenvectors.

# Results
<p float="center">
  <img src= "https://github.com/bertaye/Spectral-Based-Mesh-Segmentation/assets/39909689/0d83d03b-05f0-48cb-8278-3cc22f4f75be" width="350" />
  <img src= "https://github.com/bertaye/Spectral-Based-Mesh-Segmentation/assets/39909689/7e803373-78a3-4edd-86f4-0f972793b4ad" width="350" />
</p>
<p float="center">
  <img src= "https://github.com/bertaye/Spectral-Based-Mesh-Segmentation/assets/39909689/c7ea4beb-cea4-4f68-be20-6716f51e2d23" width="350" />
  <img src= "https://github.com/bertaye/Spectral-Based-Mesh-Segmentation/assets/39909689/cd80117c-5392-44b7-918a-d67b10935db7" width="350" />
</p>
<p float="center">
  <img src= "https://github.com/bertaye/Spectral-Based-Mesh-Segmentation/assets/39909689/a16b9bd7-11be-49fc-8e78-c042077d55b2" width="350" />
  <img src= "https://github.com/bertaye/Spectral-Based-Mesh-Segmentation/assets/39909689/171764f3-59e6-4f21-8b8b-2223a1386790" width="350" />
</p>

[EigenLib]:https://eigen.tuxfamily.org/
[SpectraLib]:https://spectralib.org/
[MyPaper]:https://github.com/bertaye/Spectral-Based-Mesh-Segmentation/blob/master/Spectral_Based_Mesh_Segmentation.pdf
