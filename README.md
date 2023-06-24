# Spectral-Based-Mesh-Segmentation
Spectral Based Mesh Segmentation build on top of a custom simple self-made OpenGL framework

A Graph Theory course term project.
Inspired from this paper:

`Mejia, D., Ruiz-Salguero, O., & Cadavid, C. A. (2017). Spectral-based mesh segmentation. International Journal on Interactive Design and Manufacturing (IJIDeM), 11, 503-514.`

A simple mesh segmentation framework relies on the Laplacian Matrix. 
Scroll down to see results

# How it works?
If you want to dive deep on the topic you definitely should read the [paper][MyPaper] I uploaded. To summarize, we are creating the Laplacian Matrix of the mesh and 
then we are finding its second smallest Eigenvalue and corresponding Eigenvector, thanks to amazing [Eigen][EigenLib] and [Spectra][SpectraLib] libraries,we can
store sparse matrices (w/Eigen), which results in efficient memory usage, and can calculate only the first two smallest eigenvectors (w/Spectra) by avoiding spending time on trivial Eigenvectors.

#Results
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
